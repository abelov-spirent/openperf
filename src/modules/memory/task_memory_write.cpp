#include "memory/task_memory_write.hpp"

#include <cassert>
#include <cstring>

namespace openperf::memory::internal {

void task_memory_write::operation(uint64_t nb_ops)
{
    assert(m_op_index < m_indexes.size());
    for (size_t i = 0; i < nb_ops; ++i) {
        unsigned idx = m_indexes.at(m_op_index++);
        std::memcpy(m_buffer + (idx * m_config.block_size),
                    m_scratch.ptr,
                    m_config.block_size);
        if (m_op_index == m_indexes.size()) { m_op_index = 0; }
    }
}

} // namespace openperf::memory::internal
