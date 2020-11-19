#ifndef _OP_NETWORK_FIREHOSE_SERVER_TCP_HPP_
#define _OP_NETWORK_FIREHOSE_SERVER_TCP_HPP_

#include <atomic>
#include <thread>
#include <vector>
#include <zmq.h>

#include "server_common.hpp"

namespace openperf::network::internal::firehose {

class server_tcp : public server
{
private:
    struct zmq_ctx_deleter
    {
        void operator()(void* ctx) const
        {
            zmq_ctx_shutdown(ctx);
            zmq_ctx_term(ctx);
        }
    };

    std::atomic_bool m_stopped;
    std::thread m_accept_thread;
    std::vector<std::unique_ptr<std::thread>> m_worker_threads;
    void* m_context;

    int tcp_write(connection_t&, std::vector<uint8_t> send_buffer);
    int new_server(int domain, in_port_t port);

public:
    server_tcp(in_port_t port, drivers::network_driver_ptr& driver);
    server_tcp(const server_tcp&) = delete;
    ~server_tcp() override;

    void run_accept_thread() override;
    void run_worker_thread() override;
};

} // namespace openperf::network::internal::firehose

#endif