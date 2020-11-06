#include "server_tcp.hpp"

#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <core/op_log.h>
#include <core/op_thread.h>
#include "core/op_socket.h"

#include "protocol.hpp"

namespace openperf::network::internal::firehose {

const static size_t tcp_backlog = 128;
const std::string endpoint = "inproc://firehose_server";

struct connection_msg_t
{
    int fd;
    union
    {
        struct sockaddr client;
        struct sockaddr_in client4;
        struct sockaddr_in6 client6;
    };
};

static int _server(int domain, in_port_t port)
{
    struct sockaddr_storage client_storage;
    struct sockaddr* server_ptr = (struct sockaddr*)&client_storage;
    std::string domain_str;

    switch (domain) {
    case AF_INET: {
        struct sockaddr_in* server4 = (struct sockaddr_in*)server_ptr;
        server4->sin_family = AF_INET;
        server4->sin_port = htons(port);
        server4->sin_addr.s_addr = htonl(INADDR_ANY);
        domain_str = "IPv4";
        break;
    }
    case AF_INET6: {
        struct sockaddr_in6* server6 = (struct sockaddr_in6*)server_ptr;
        server6->sin6_family = AF_INET6;
        server6->sin6_port = htons(port);
        server6->sin6_addr = in6addr_any;
        domain_str = "IPv6";
        break;
    }
    default:
        return (-EINVAL);
    }

    int sock = 0, enable = true;
    if ((sock = socket(domain, SOCK_STREAM, 0)) == -1) {
        OP_LOG(OP_LOG_WARNING,
               "Unable to open %s UDP server socket: %s\n",
               domain_str.c_str(),
               strerror(errno));
        return -1;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable))
        != 0) {
        ::close(sock);
        return (-1);
    }

    if (bind(sock, server_ptr, get_sa_len(server_ptr)) == -1) {
        OP_LOG(OP_LOG_ERROR,
               "Unable to bind to socket (domain = %d, protocol = TCP): %s\n",
               domain,
               strerror(errno));
    }

    if (listen(sock, tcp_backlog) == -1) {
        OP_LOG(OP_LOG_ERROR,
               "Unable to listen on socket %d: %s\n",
               sock,
               strerror(errno));
    }

    return (sock);
}

server_tcp::server_tcp(in_port_t port)
    : m_stopped(false)
    , m_context(zmq_ctx_new())
{
    /* IPv6 any supports IPv4 and IPv6 */
    if ((m_fd = _server(AF_INET6, port)) >= 0) {
        OP_LOG(OP_LOG_INFO, "Network TCP load server IPv4/IPv6.\n");
    } else {
        /* Couldn't bind IPv6 socket so use IPv4 */
        if ((m_fd = _server(AF_INET, port)) < 0) { return; }
        OP_LOG(OP_LOG_INFO, "Network TCP load server IPv4.\n");
    }
}

server_tcp::~server_tcp()
{
    m_stopped.store(true, std::memory_order_relaxed);
    zmq_ctx_shutdown(m_context);
    if (m_fd.load() >= 0) shutdown(m_fd.load(), SHUT_RDWR);

    if (m_accept_thread.joinable()) m_accept_thread.join();
    for (auto& thread : m_worker_threads) {
        if (thread->joinable()) thread->join();
    }
}

void server_tcp::run_accept_thread()
{
    if (m_accept_thread.joinable()) return;

    m_accept_thread = std::thread([&] {
        // Set the thread name
        op_thread_setname("op_net_srv_acc");

        // Run the loop of the thread
        struct sockaddr_storage client_storage;
        struct sockaddr* client = (struct sockaddr*)&client_storage;
        socklen_t client_length = sizeof(client_storage);

        void* sync = op_socket_get_server(m_context, ZMQ_PUSH, endpoint.data());

        int connect_fd;
        while ((connect_fd = accept(m_fd.load(), client, &client_length))
               != -1) {
            int enable = 1;
            if (setsockopt(connect_fd,
                           IPPROTO_TCP,
                           TCP_NODELAY,
                           &enable,
                           sizeof(enable))
                != 0) {
                close(connect_fd);
                continue;
            }
            // Change the socket into non-blocking state
            int flags;
            if ((flags = fcntl(connect_fd, F_GETFL, 0)); flags >= 0) {
                flags |= O_NONBLOCK;
                if (fcntl(connect_fd, F_SETFL, O_NONBLOCK)) {
                    OP_LOG(OP_LOG_WARNING,
                           "Cannot set non blocking socket (%d): %s\n",
                           connect_fd,
                           strerror(errno));
                }
            } else {
                OP_LOG(OP_LOG_WARNING,
                       "Cannot get file descriptor flags (%d): %s\n",
                       connect_fd,
                       strerror(errno));
            }

            auto conn = connection_msg_t{.fd = connect_fd};
            memcpy(&conn.client, client, get_sa_len(client));
            zmq_send(sync, &conn, sizeof(conn), ZMQ_DONTWAIT);
        }
    });
}

int server_tcp::tcp_write(connection_t& conn)
{

    int flags = 0;
#if defined(MSG_NOSIGNAL)
    flags |= MSG_NOSIGNAL;
#endif

    if (conn.state != STATE_WRITING) { return 0; }

    assert(conn.to_write);

    size_t to_send = std::min(send_buffer_size, conn.to_write);
    ssize_t send_or_err = send(conn.fd, m_send_buffer.data(), to_send, flags);
    if (send_or_err == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) { return 0; }

        char ntopbuf[INET6_ADDRSTRLEN];
        const char* addr = inet_ntop(conn.client.sa_family,
                                     get_sa_addr(&conn.client),
                                     ntopbuf,
                                     INET6_ADDRSTRLEN);
        OP_LOG(OP_LOG_ERROR,
               "Error sending to %s:%d: %s\n",
               addr ? addr : "unknown",
               ntohs(get_sa_port(&conn.client)),
               strerror(errno));
        conn.state = STATE_ERROR;
        return -1;
    }

    conn.to_write -= send_or_err;

    conn.state = STATE_WAITING;

    return 0;
}

void server_tcp::run_worker_thread()
{
    m_worker_threads.push_back(std::make_unique<std::thread>([&] {
        // Set the thread name
        op_thread_setname("op_net_srv_w");
        std::vector<connection_t> connections;
        connection_msg_t conn_buffer;
        void* sync = op_socket_get_client(m_context, ZMQ_PULL, endpoint.data());

        while (!m_stopped.load(std::memory_order_relaxed)) {
            // Receive all accepted connections

            while (zmq_recv(sync,
                            &conn_buffer,
                            sizeof(conn_buffer),
                            (connections.size() > 0) ? ZMQ_DONTWAIT : 0)
                   >= 0) {
                auto conn = connection_t{
                    .fd = conn_buffer.fd,
                    .state = STATE_WAITING,
                };
                memcpy(&conn.client,
                       &conn_buffer.client,
                       get_sa_len(&conn_buffer.client));
                connections.push_back(conn);

                OP_LOG(OP_LOG_INFO,
                       "SERVER: Received new connection %d, total %zu\n",
                       conn.fd,
                       connections.size());
            }

            for (auto& conn : connections) {
                uint8_t recv_buffer[recv_buffer_size];
                size_t recv_length = recv_buffer_size;
                uint8_t* recv_cursor = &recv_buffer[0];

                ssize_t recv_or_err = 0;
                do {
                    if (conn.state == STATE_WRITING) {
                        tcp_write(conn);
                        break;
                    }

                    if ((recv_or_err =
                             recv(conn.fd, recv_cursor, recv_length, 0))
                        == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) { break; }

                        char ntopbuf[INET6_ADDRSTRLEN];
                        const char* addr = inet_ntop(conn.client.sa_family,
                                                     get_sa_addr(&conn.client),
                                                     ntopbuf,
                                                     INET6_ADDRSTRLEN);
                        OP_LOG(OP_LOG_ERROR,
                               "Error reading from %s:%d: %s\n",
                               addr ? addr : "unknown",
                               ntohs(get_sa_port(&conn.client)),
                               strerror(errno));
                        conn.state = STATE_ERROR;
                        break;
                    } else if (recv_or_err == 0) {
                        conn.state = STATE_DONE;
                        break;
                    }

                    if (conn.request.size()) {
                        /* put the cursor back in the right spot */
                        recv_cursor -= conn.request.size();
                        recv_or_err += conn.request.size();
                        conn.request.clear();
                    }

                    size_t bytes_left = recv_or_err;
                    while (bytes_left) {
                        switch (conn.state) {
                        case STATE_WAITING: {
                            if (bytes_left < firehose::net_request_size) {
                                /* need to wait for more header */
                                conn.request.insert(conn.request.end(),
                                                    recv_cursor,
                                                    recv_cursor + bytes_left);
                                bytes_left = 0;
                                break;
                            }

                            /* Note: this function updates the note,
                             * including the state */
                            auto req = firehose::parse_request(recv_cursor,
                                                               bytes_left);
                            if (!req) {
                                char ntopbuf[INET6_ADDRSTRLEN];
                                const char* addr =
                                    inet_ntop(conn.client.sa_family,
                                              get_sa_addr(&conn.client),
                                              ntopbuf,
                                              INET6_ADDRSTRLEN);
                                OP_LOG(OP_LOG_ERROR,
                                       "Invalid firehose request received "
                                       "from %s:%d\n",
                                       addr ? addr : "unknown",
                                       ntohs(get_sa_port(&conn.client)));
                                conn.state = STATE_ERROR;
                            }
                            break;
                        }
                        case STATE_READING: {
                            size_t consumed =
                                std::min(bytes_left, conn.to_read);
                            conn.to_read -= consumed;
                            bytes_left -= consumed;
                            recv_cursor += consumed;

                            if (conn.to_read == 0) {
                                conn.state = STATE_WAITING;
                            }

                            break;
                        }
                        default: {
                            char ntopbuf[INET6_ADDRSTRLEN];
                            const char* addr =
                                inet_ntop(conn.client.sa_family,
                                          get_sa_addr(&conn.client),
                                          ntopbuf,
                                          INET6_ADDRSTRLEN);
                            OP_LOG(OP_LOG_WARNING,
                                   "Connection from %s:%d is in state %s"
                                   " with %zu bytes left to read. Dropping "
                                   "connection.\n",
                                   addr ? addr : "unknown",
                                   ntohs(get_sa_port(&conn.client)),
                                   get_state_string(conn.state),
                                   bytes_left);
                            conn.state = STATE_ERROR;
                            break;
                        }
                        }
                    }
                } while (recv_or_err == recv_buffer_size);
            }

            // Remove closed connections
            auto s = connections.size();
            connections.erase(std::remove_if(connections.begin(),
                                             connections.end(),
                                             [](const auto& conn) {
                                                 return conn.state == STATE_DONE
                                                        || conn.state
                                                               == STATE_ERROR;
                                             }),
                              connections.end());
            if (connections.size() != s) {
                OP_LOG(OP_LOG_INFO,
                       "SERVER: Removed %zu connections, current %zu\n",
                       s - connections.size(),
                       connections.size());
            }
        }
    }));
}

} // namespace openperf::network::internal::firehose