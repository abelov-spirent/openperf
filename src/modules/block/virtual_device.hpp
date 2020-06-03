#ifndef _OP_BLOCK_VIRTUAL_DEVICE_HPP_
#define _OP_BLOCK_VIRTUAL_DEVICE_HPP_

#include <stdexcept>
#include <atomic>
#include <thread>
#include <tl/expected.hpp>
#include "timesync/bintime.hpp"

namespace openperf::block {

#define VIRTUAL_DEVICE_HEADER_TAG "This is a big, fat OP VDEV header tag!"
#define VIRTUAL_DEVICE_HEADER_TAG_LENGTH 40
#define VIRTUAL_DEVICE_HEADER_PAD_LENGTH                                       \
    512 - VIRTUAL_DEVICE_HEADER_TAG_LENGTH - sizeof(timesync::bintime)         \
        - sizeof(size_t)

struct virtual_device_header
{
    char tag[VIRTUAL_DEVICE_HEADER_TAG_LENGTH];
    timesync::bintime init_time;
    size_t size;
    uint8_t pad[VIRTUAL_DEVICE_HEADER_PAD_LENGTH];
} __attribute__((packed));

struct virtual_device_descriptors
{
    int read = -1;
    int write = -1;
};

const size_t block_generator_vdev_header_size = sizeof(virtual_device_header);

class virtual_device
{
protected:
    int m_read_fd = -1, m_write_fd = -1;
    std::thread m_scrub_thread;
    std::atomic_bool m_deleted;
    int write_header(int fd);
    void scrub_worker(size_t start, size_t stop);
    virtual void scrub_done(){};
    virtual void scrub_update(double){};

public:
    virtual_device();
    virtual ~virtual_device();
    virtual int open(int flags) = 0;
    virtual int close(int fd) = 0;
    virtual tl::expected<virtual_device_descriptors, int> vopen() = 0;
    virtual void vclose() = 0;
    void queue_scrub();
    void terminate_scrub();
    virtual uint64_t get_size() const = 0;
    virtual uint64_t get_header_size() const { return 0; };
    std::optional<virtual_device_descriptors> get_fd() const
    {
        if (m_read_fd < 0 || m_write_fd < 0) return std::nullopt;
        return (virtual_device_descriptors){
            m_read_fd,
            m_write_fd
        };
    }
};

class virtual_device_stack
{
public:
    virtual ~virtual_device_stack() = default;
    virtual std::shared_ptr<virtual_device>
    get_vdev(const std::string&) const = 0;
};

} // namespace openperf::block

#endif