#ifndef _OP_TVLP_NETWORK_WORKER_HPP_
#define _OP_TVLP_NETWORK_WORKER_HPP_

#include "tvlp/worker.hpp"

namespace openperf::tvlp::internal::worker {

class network_tvlp_worker_t : public tvlp_worker_t
{
public:
    network_tvlp_worker_t() = delete;
    network_tvlp_worker_t(const network_tvlp_worker_t&) = delete;
    network_tvlp_worker_t(void* context, const model::tvlp_module_profile_t&);
    ~network_tvlp_worker_t();

protected:
    tl::expected<std::string, std::string>
    send_create(const model::tvlp_profile_entry_t&) override;
    tl::expected<stat_pair_t, std::string>
    send_start(const std::string& id,
               const dynamic::configuration& dynamic_results) override;
    tl::expected<void, std::string> send_stop(const std::string& id) override;
    tl::expected<nlohmann::json, std::string>
    send_stat(const std::string& id) override;
    tl::expected<void, std::string> send_delete(const std::string& id) override;
};

} // namespace openperf::tvlp::internal::worker

#endif