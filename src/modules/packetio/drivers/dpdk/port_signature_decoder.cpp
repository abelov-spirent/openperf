#include <algorithm>

#include "core/op_log.h"
#include "packetio/drivers/dpdk/dpdk.h"
#include "packetio/drivers/dpdk/model/port_info.hpp"
#include "packetio/drivers/dpdk/mbuf_signature.hpp"
#include "packetio/drivers/dpdk/port_signature_decoder.hpp"
#include "spirent_pga/api.h"
#include "utils/soa_container.hpp"

namespace openperf::packetio::dpdk::port {

static constexpr auto chunk_size = 32U;
static constexpr auto signature_size = 20U;

template <typename T> using chunk_array = std::array<T, chunk_size>;

static uint16_t detect_signatures([[maybe_unused]] uint16_t port_id,
                                  [[maybe_unused]] uint16_t queue_id,
                                  rte_mbuf* packets[],
                                  uint16_t nb_packets,
                                  [[maybe_unused]] uint16_t max_packets,
                                  [[maybe_unused]] void* user_param)
{
    using payload_container =
        utils::soa_container<chunk_array,
                             std::tuple<const uint8_t*, std::byte[20]>>;
    using sig_container =
        utils::soa_container<chunk_array,
                             std::tuple<uint32_t, uint32_t, uint64_t, int>>;

    payload_container payloads;
    sig_container signatures;
    auto crc_matches = std::array<int, chunk_size>{};

    auto start = 0U;
    while (start < nb_packets) {
        auto end = start + std::min(chunk_size, nb_packets - start);
        auto count = end - start;

        /*
         * The spirent signature should start 20 bytes before the end of the
         * packet, so generate an array of expected locations to check.
         */
        std::transform(packets + start,
                       packets + end,
                       payloads.data<1>(), /* storage */
                       payloads.data<0>(), /* pointer */
                       [&](const auto& mbuf, auto& storage) {
                           return (static_cast<const uint8_t*>(rte_pktmbuf_read(
                               mbuf,
                               rte_pktmbuf_pkt_len(mbuf) - signature_size,
                               signature_size,
                               storage)));
                       });

        /* Look for signature candidates */
        if (pga_signatures_crc_filter(
                payloads.data<0>(), count, crc_matches.data())) {

            /* Matches found; decode signatures */
            pga_signatures_decode(payloads.data<0>(),
                                  count,
                                  signatures.data<0>(),  /* stream id */
                                  signatures.data<1>(),  /* sequence num */
                                  signatures.data<2>(),  /* timestamp */
                                  signatures.data<3>()); /* flags */

            /* Write valid signature data to the associated mbuf */
            for (auto idx = 0U; idx < count; idx++) {
                const auto& sig = signatures[idx];
                if (crc_matches[idx]
                    && (pga_status_flag(std::get<3>(sig))
                        == pga_signature_status::valid)) {
                    mbuf_signature_set(packets[start + idx],
                                       std::get<0>(sig),
                                       std::get<1>(sig),
                                       std::get<2>(sig),
                                       std::get<3>(sig));
                }
            }
        }

        start = end;
    }

    return (nb_packets);
}

callback_signature_decoder::callback_signature_decoder(uint16_t port_id)
    : m_port(port_id)
{}

callback_signature_decoder::~callback_signature_decoder()
{
    if (m_callbacks.empty()) return;

    std::for_each(
        std::begin(m_callbacks), std::end(m_callbacks), [&](auto& item) {
            rte_eth_remove_rx_callback(port_id(), item.first, item.second);
        });
}

callback_signature_decoder::callback_signature_decoder(
    callback_signature_decoder&& other)
    : m_callbacks(std::move(other.m_callbacks))
    , m_port(other.m_port)
{}

callback_signature_decoder&
callback_signature_decoder::operator=(callback_signature_decoder&& other)
{
    if (this != &other) {
        m_callbacks = std::move(other.m_callbacks);
        m_port = other.m_port;
    }

    return (*this);
}

uint16_t callback_signature_decoder::port_id() const { return (m_port); }

void callback_signature_decoder::enable()
{
    OP_LOG(OP_LOG_INFO,
           "Enabling software Spirent signature detection on port %u\n",
           m_port);

    for (auto q = 0U; q < model::port_info(port_id()).rx_queue_count(); q++) {
        auto cb =
            rte_eth_add_rx_callback(port_id(), q, detect_signatures, nullptr);
        if (!cb) {
            throw std::runtime_error("Could not add timestamp callback");
        }
        m_callbacks.emplace(q, cb);
    }
}

void callback_signature_decoder::disable()
{
    OP_LOG(OP_LOG_INFO,
           "Disabling software Spirent signature detection on port %u\n",
           m_port);

    std::for_each(
        std::begin(m_callbacks), std::end(m_callbacks), [&](auto& item) {
            rte_eth_remove_rx_callback(port_id(), item.first, item.second);
        });
}

static signature_decoder::variant_type make_signature_decoder(uint16_t port_id)
{
    return (callback_signature_decoder(port_id));
}

signature_decoder::signature_decoder(uint16_t port_id)
    : feature(make_signature_decoder(port_id))
{}

} // namespace openperf::packetio::dpdk::port