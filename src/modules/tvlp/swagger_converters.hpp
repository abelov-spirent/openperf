#ifndef _OP_MEMORY_SWAGGER_CONVERTERS_HPP_
#define _OP_MEMORY_SWAGGER_CONVERTERS_HPP_

#include <iomanip>

#include "models/tvlp_config.hpp"
#include "swagger/converters/block.hpp"
#include "swagger/converters/memory.hpp"
#include "swagger/converters/cpu.hpp"
#include "swagger/converters/packet_generator.hpp"
#include "swagger/v1/model/TvlpConfiguration.h"
#include "swagger/v1/model/TvlpResult.h"

namespace openperf::tvlp::api {

using config_t = tvlp::model::tvlp_configuration_t;
using profile_t = tvlp::model::tvlp_profile_t;
using profile_entry_t = tvlp::model::tvlp_profile_entry_t;
using result_t = tvlp::model::tvlp_result_t;

namespace swagger = ::swagger::v1::model;

template <typename Rep, typename Period>
std::string to_rfc3339(std::chrono::duration<Rep, Period> from)
{
    auto tv = openperf::timesync::to_timeval(from);
    std::stringstream os;
    os << std::put_time(gmtime(&tv.tv_sec), "%FT%T") << "." << std::setfill('0')
       << std::setw(6) << tv.tv_usec << "Z";
    return (os.str());
}

static config_t from_swagger(const swagger::TvlpConfiguration& m)
{
    auto config = config_t{};
    config.id(m.getId());
    profile_t profile;
    if (m.getProfile()->blockIsSet()) {
        profile.block = std::vector<profile_entry_t>();
        for (auto block_entry : m.getProfile()->getBlock()->getSeries()) {
            profile.block.value().push_back(profile_entry_t{
                .length = model::duration(block_entry->getLength()),
                .resource_id = block_entry->getResourceId(),
                .config = block_entry->getConfig()->toJson()});
        }
    }
    if (m.getProfile()->memoryIsSet()) {
        profile.memory = std::vector<profile_entry_t>();
        for (auto memory_entry : m.getProfile()->getMemory()->getSeries()) {
            profile.memory.value().push_back(profile_entry_t{
                .length = model::duration(memory_entry->getLength()),
                .config = memory_entry->getConfig()->toJson()});
        }
    }
    if (m.getProfile()->cpuIsSet()) {
        profile.cpu = std::vector<profile_entry_t>();
        for (auto cpu_entry : m.getProfile()->getCpu()->getSeries()) {
            profile.cpu.value().push_back(profile_entry_t{
                .length = model::duration(cpu_entry->getLength()),
                .config = cpu_entry->getConfig()->toJson()});
        }
    }
    if (m.getProfile()->packetIsSet()) {
        profile.packet = std::vector<profile_entry_t>();
        for (auto packet_entry : m.getProfile()->getPacket()->getSeries()) {
            profile.packet.value().push_back(profile_entry_t{
                .length = model::duration(packet_entry->getLength()),
                .resource_id = packet_entry->getTargetId(),
                .config = packet_entry->getConfig()->toJson()});
        }
    }
    config.profile(profile);
    return config;
}

static swagger::TvlpConfiguration to_swagger(const config_t& config)
{
    swagger::TvlpConfiguration model;
    model.setId(config.id());
    model.setTime(std::make_shared<swagger::TvlpConfiguration_time>());
    model.getTime()->setLength(config.total_length().count());
    if (config.state() == model::RUNNING || config.state() == model::COUNTDOWN)
        model.getTime()->setStart(
            to_rfc3339(config.start_time().time_since_epoch()));
    if (config.state() == model::RUNNING)
        model.getTime()->setOffset(config.current_offset().count());
    if (config.state() == model::ERROR) model.setError(config.error());
    switch (config.state()) {
    case (model::READY):
        model.setState("ready");
        break;
    case (model::COUNTDOWN):
        model.setState("countdown");
        break;
    case (model::RUNNING):
        model.setState("running");
        break;
    case (model::ERROR):
        model.setState("error");
    }

    model.setProfile(std::make_shared<swagger::TvlpProfile>());
    if (config.profile().block) {
        auto block = std::make_shared<swagger::TvlpProfile_block>();

        auto block_vector = config.profile().block.value();
        std::for_each(
            std::begin(block_vector),
            std::end(block_vector),
            [&](auto& block_entry) {
                auto entry =
                    std::make_shared<swagger::TvlpProfile_block_series>();
                entry->setLength(block_entry.length.count());
                entry->setResourceId(block_entry.resource_id.value());

                auto g_config =
                    std::make_shared<swagger::BlockGeneratorConfig>();
                g_config->fromJson(block_entry.config);
                entry->setConfig(g_config);

                block->getSeries().push_back(entry);
            });

        model.getProfile()->setBlock(block);
    }
    if (config.profile().memory) {
        auto memory = std::make_shared<swagger::TvlpProfile_memory>();

        auto memory_vector = config.profile().memory.value();
        std::for_each(
            std::begin(memory_vector),
            std::end(memory_vector),
            [&](auto& memory_entry) {
                auto entry =
                    std::make_shared<swagger::TvlpProfile_memory_series>();
                entry->setLength(memory_entry.length.count());

                auto g_config =
                    std::make_shared<swagger::MemoryGeneratorConfig>();
                g_config->fromJson(memory_entry.config);
                entry->setConfig(g_config);

                memory->getSeries().push_back(entry);
            });

        model.getProfile()->setMemory(memory);
    }
    if (config.profile().cpu) {
        auto cpu = std::make_shared<swagger::TvlpProfile_cpu>();

        auto cpu_vector = config.profile().memory.value();
        std::for_each(
            std::begin(cpu_vector), std::end(cpu_vector), [&](auto& cpu_entry) {
                auto entry =
                    std::make_shared<swagger::TvlpProfile_cpu_series>();
                entry->setLength(cpu_entry.length.count());

                auto g_config = std::make_shared<swagger::CpuGeneratorConfig>();
                g_config->fromJson(cpu_entry.config);
                entry->setConfig(g_config);

                cpu->getSeries().push_back(entry);
            });

        model.getProfile()->setCpu(cpu);
    }
    if (config.profile().packet) {
        auto packet = std::make_shared<swagger::TvlpProfile_packet>();

        auto packet_vector = config.profile().memory.value();
        std::for_each(
            std::begin(packet_vector),
            std::end(packet_vector),
            [&](auto& packet_entry) {
                auto entry =
                    std::make_shared<swagger::TvlpProfile_packet_series>();
                entry->setLength(packet_entry.length.count());
                entry->setTargetId(packet_entry.resource_id.value());

                auto g_config =
                    std::make_shared<swagger::PacketGeneratorConfig>();
                swagger::from_json(packet_entry.config, *g_config);
                entry->setConfig(g_config);

                packet->getSeries().push_back(entry);
            });

        model.getProfile()->setPacket(packet);
    }

    return model;
}

static swagger::TvlpResult to_swagger(const result_t& result)
{
    swagger::TvlpResult model;
    model.setId(result.id());
    model.setTvlpId(result.tvlp_id());

    auto modules_results = result.results();
    if (modules_results.block) {
        auto block_results = modules_results.block.value();
        std::for_each(std::begin(block_results),
                      std::end(block_results),
                      [&](const auto& res) {
                          auto g_result =
                              std::make_shared<swagger::BlockGeneratorResult>();
                          swagger::from_json(res, *g_result);
                          model.getBlock().push_back(g_result);
                      });
    }
    if (modules_results.memory) {
        auto memory_results = modules_results.memory.value();
        std::for_each(
            std::begin(memory_results),
            std::end(memory_results),
            [&](const auto& res) {
                auto g_result =
                    std::make_shared<swagger::MemoryGeneratorResult>();
                swagger::from_json(res, *g_result);
                model.getMemory().push_back(g_result);
            });
    }
    if (modules_results.cpu) {
        auto cpu_results = modules_results.cpu.value();
        std::for_each(std::begin(cpu_results),
                      std::end(cpu_results),
                      [&](const auto& res) {
                          auto g_result =
                              std::make_shared<swagger::CpuGeneratorResult>();
                          swagger::from_json(res, *g_result);
                          model.getCpu().push_back(g_result);
                      });
    }
    if (modules_results.packet) {
        auto packet_results = modules_results.packet.value();
        std::for_each(
            std::begin(packet_results),
            std::end(packet_results),
            [&](const auto& res) {
                auto g_result =
                    std::make_shared<swagger::PacketGeneratorResult>();
                swagger::from_json(res, *g_result);
                model.getPacket().push_back(g_result);
            });
    }

    return model;
}

} // namespace openperf::tvlp::api

#endif // _OP_MEMORY_SWAGGER_CONVERTERS_HPP_