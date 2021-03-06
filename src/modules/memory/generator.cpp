#include "memory/generator.hpp"
#include "memory/task_memory_read.hpp"
#include "memory/task_memory_write.hpp"

#include <cinttypes>
#include <sys/mman.h>

namespace openperf::memory::internal {

static uint16_t serial_counter = 0;

// Constructors & Destructor
generator::generator()
    : m_buffer{.ptr = nullptr, .size = 0}
    , m_serial_number(++serial_counter)
{}

generator::generator(generator&& g) noexcept
    : m_stopped(g.m_stopped)
    , m_paused(g.m_paused)
    , m_read_workers(std::move(g.m_read_workers))
    , m_write_workers(std::move(g.m_write_workers))
    , m_config(g.m_config)
    , m_buffer{.ptr = nullptr, .size = 0}
    , m_serial_number(g.m_serial_number)
{}

generator& generator::operator=(generator&& g) noexcept
{
    if (this != &g) {
        m_stopped = g.m_stopped;
        m_paused = g.m_paused;
        m_read_workers = std::move(g.m_read_workers);
        m_write_workers = std::move(g.m_write_workers);
        m_config = g.m_config;
        m_buffer = {.ptr = nullptr, .size = 0};
        m_serial_number = g.m_serial_number;
    }
    return (*this);
}

generator::generator(const generator::config_t& c)
    : generator()
{
    config(c);
}

generator::~generator()
{
    stop();
    free_buffer();
}

// Methods : public
void generator::start()
{
    if (!m_stopped) return;

    for_each_worker([](worker_ptr& w) { w->start(); });
    m_stopped = false;
    m_run_time = 0ms;
    m_run_time_milestone = std::chrono::system_clock::now();
}

void generator::stop()
{
    if (m_stopped) return;

    for_each_worker([](worker_ptr& w) { w->stop(); });
    m_stopped = true;
    m_run_time += std::chrono::system_clock::now() - m_run_time_milestone;
}

void generator::restart()
{
    stop();
    start();
}

void generator::resume()
{
    if (!m_paused) return;

    for_each_worker([](worker_ptr& w) { w->resume(); });
    m_paused = false;
    m_run_time_milestone = std::chrono::system_clock::now();
}

void generator::pause()
{
    if (m_paused) return;

    for_each_worker([](worker_ptr& w) { w->pause(); });
    m_paused = true;
    m_run_time += std::chrono::system_clock::now() - m_run_time_milestone;
}

void generator::reset()
{
    for_each_worker([](worker_ptr& w) {
        auto wtm = reinterpret_cast<worker<task_memory>*>(w.get());
        wtm->clear_stat();
    });
}

generator::stat_t generator::stat() const
{
    auto elapsed_time = m_run_time;
    if (is_running())
        elapsed_time += std::chrono::system_clock::now() - m_run_time_milestone;

    generator::stat_t result_stat;

    auto& rstat = result_stat.read;
    if (!m_read_workers.empty()) {
        for (const auto& ptr : m_read_workers) {
            auto w = reinterpret_cast<worker<task_memory>*>(ptr.get());
            rstat += w->stat();
        }

        rstat.operations_target =
            elapsed_time.count() * m_config.read.op_per_sec
            * std::min(m_config.read.block_size, 1UL) / std::nano::den;
        rstat.bytes_target = rstat.operations_target * m_config.read.block_size;
    }

    auto& wstat = result_stat.write;
    if (!m_write_workers.empty()) {
        for (const auto& ptr : m_write_workers) {
            auto w = reinterpret_cast<worker<task_memory>*>(ptr.get());
            wstat += w->stat();
        }

        wstat.operations_target =
            elapsed_time.count() * m_config.write.op_per_sec
            * std::min(m_config.write.block_size, 1UL) / std::nano::den;
        wstat.bytes_target =
            wstat.operations_target * m_config.write.block_size;
    }

    result_stat.timestamp = std::max(rstat.timestamp, wstat.timestamp);
    result_stat.active = is_running();
    return result_stat;
}

generator::config_t generator::config() const { return m_config; }

void generator::config(const generator::config_t& cfg)
{
    pause();
    resize_buffer(cfg.buffer_size);

    reallocate_workers<task_memory_read>(m_read_workers, cfg.read_threads);

    auto read_rate =
        (!cfg.read_threads) ? 0 : cfg.read.op_per_sec / cfg.read_threads;

    spread_config(m_read_workers,
                  task_memory_config{
                      .block_size = cfg.read.block_size,
                      .op_per_sec = read_rate,
                      .pattern = cfg.read.pattern,
                      .buffer = {.ptr = m_buffer.ptr, .size = m_buffer.size}});

    reallocate_workers<task_memory_write>(m_write_workers, cfg.write_threads);

    auto write_rate =
        (!cfg.write_threads) ? 0 : cfg.write.op_per_sec / cfg.write_threads;

    spread_config(m_write_workers,
                  task_memory_config{
                      .block_size = cfg.write.block_size,
                      .op_per_sec = write_rate,
                      .pattern = cfg.write.pattern,
                      .buffer = {.ptr = m_buffer.ptr, .size = m_buffer.size}});

    m_config = cfg;
    resume();
}

// Methods : private
void generator::free_buffer()
{
    if (m_buffer.ptr == nullptr) return;
    if (m_buffer.ptr == MAP_FAILED) return;

    if (munmap(m_buffer.ptr, m_buffer.size) == -1) {
        OP_LOG(OP_LOG_ERROR,
               "Failed to unallocate %zu"
               " bytes of memory: %s\n",
               m_buffer.size,
               strerror(errno));
    }

    m_buffer.ptr = MAP_FAILED;
    m_buffer.size = 0;
}

void generator::resize_buffer(size_t size)
{
    if (size == m_buffer.size) return;

    /*
     * We use mmap/munmap here instead of the standard allocation functions
     * because we expect this buffer to be relatively large, and OSv currently
     * can only allocate non-contiguous chunks of memory with mmap.
     * The buffer contents don't currently matter, so there is no reason to
     * initialize new buffers or shuffle contents between old and new ones.
     */
    free_buffer();
    if (size > 0) {
        OP_LOG(OP_LOG_INFO,
               "Reallocating buffer (%zu => %zu)\n",
               m_buffer.size,
               size);

        m_buffer.ptr = mmap(nullptr,
                            size,
                            PROT_READ | PROT_WRITE,
                            MAP_ANONYMOUS | MAP_PRIVATE,
                            -1,
                            0);

        if (m_buffer.ptr == MAP_FAILED) {
            OP_LOG(OP_LOG_ERROR,
                   "Failed to allocate %" PRIu64 " byte memory buffer\n",
                   size);
            return;
        }

        m_buffer.size = size;
    }
}

void generator::spread_config(generator::workers& wkrs,
                              const task_memory_config& config)
{
    for (auto& w : wkrs) {
        auto wt = reinterpret_cast<worker<task_memory>*>(w.get());
        wt->config(config);
    }
}

} // namespace openperf::memory::internal
