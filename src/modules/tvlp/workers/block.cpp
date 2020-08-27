#include "block.hpp"
#include "swagger/converters/block.hpp"
#include "swagger/v1/model/BlockGenerator.h"
#include "swagger/v1/model/BlockGeneratorResult.h"

namespace openperf::tvlp::internal::worker {

using namespace swagger::v1::model;

block_tvlp_worker_t::block_tvlp_worker_t(
    const model::tvlp_module_profile_t& profile)
    : tvlp_worker_t(profile){};

tl::expected<std::string, std::string>
block_tvlp_worker_t::send_create(const nlohmann::json& config,
                                 const std::string& resource_id)
{
    BlockGenerator gen;
    gen.setResourceId(resource_id);
    auto blk_conf = std::make_shared<BlockGeneratorConfig>();
    blk_conf->fromJson(const_cast<nlohmann::json&>(config));
    gen.setConfig(blk_conf);

    auto result = openperf::api::client::internal_api_post(
        "/block-generators", gen.toJson().dump(), INTERNAL_REQUEST_TIMEOUT);

    if (result.first < Pistache::Http::Code::Ok
        || result.first >= Pistache::Http::Code::Already_Reported)
        return tl::make_unexpected(result.second);

    auto bg = nlohmann::json::parse(result.second).get<BlockGenerator>();
    return bg.getId();
}
tl::expected<stat_pair_t, std::string>
block_tvlp_worker_t::send_start(const std::string& id)
{
    auto result = openperf::api::client::internal_api_post(
        "/block-generators/" + id + "/start", "", INTERNAL_REQUEST_TIMEOUT);
    if (result.first < Pistache::Http::Code::Ok
        || result.first >= Pistache::Http::Code::Already_Reported)
        return tl::make_unexpected(result.second);
    auto stat = nlohmann::json::parse(result.second);
    return std::pair(stat.get<BlockGeneratorResult>().getId(), stat);
}
tl::expected<void, std::string>
block_tvlp_worker_t::send_stop(const std::string& id)
{
    auto result = openperf::api::client::internal_api_post(
        "/block-generators/" + id + "/stop", "", INTERNAL_REQUEST_TIMEOUT);
    if (result.first < Pistache::Http::Code::Ok
        || result.first >= Pistache::Http::Code::Already_Reported)
        return tl::make_unexpected(result.second);
    return {};
}
tl::expected<nlohmann::json, std::string>
block_tvlp_worker_t::send_stat(const std::string& id)
{
    auto result = openperf::api::client::internal_api_get(
        "/block-generator-results/" + id, INTERNAL_REQUEST_TIMEOUT);
    if (result.first < Pistache::Http::Code::Ok
        || result.first >= Pistache::Http::Code::Already_Reported)
        return tl::make_unexpected(result.second);
    return nlohmann::json::parse(result.second);
}
tl::expected<void, std::string>
block_tvlp_worker_t::send_delete(const std::string& id)
{
    auto result = openperf::api::client::internal_api_del(
        "/block-generators/" + id, INTERNAL_REQUEST_TIMEOUT);
    if (result.first < Pistache::Http::Code::Ok
        || result.first >= Pistache::Http::Code::Already_Reported)
        return tl::make_unexpected(result.second);
    return {};
}

} // namespace openperf::tvlp::internal::worker