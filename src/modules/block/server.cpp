#include <zmq.h>

#include "block/api.hpp"
#include "block/server.hpp"
#include "config/op_config_utils.hpp"
#include "block/block_generator.hpp"
#include "utils.hpp"
#include "swagger/v1/model/BulkStartBlockGeneratorsRequest.h"
#include "swagger/v1/model/BulkStartBlockGeneratorsResponse.h"
#include "swagger/v1/model/BulkStopBlockGeneratorsRequest.h"

namespace openperf::block::api {

using namespace swagger::v1::model;
using namespace openperf::block::generator;

std::string to_string(request_type type)
{
    const static std::unordered_map<request_type, std::string> request_types = {
        {request_type::LIST_DEVICES, "LIST_INTERFACES"},
        {request_type::GET_DEVICE, "GET_DEVICE"},
        {request_type::LIST_FILES, "LIST_FILES"},
        {request_type::CREATE_FILE, "CREATE_FILE"},
        {request_type::GET_FILE, "GET_FILE"},
        {request_type::DELETE_FILE, "DELETE_FILE"},
        {request_type::LIST_GENERATORS, "LIST_GENERATORS"},
        {request_type::CREATE_GENERATOR, "CREATE_GENERATOR"},
        {request_type::GET_GENERATOR, "GET_GENERATOR"},
        {request_type::DELETE_GENERATOR, "DELETE_GENERATOR"},
        {request_type::START_GENERATOR, "START_GENERATOR"},
        {request_type::STOP_GENERATOR, "STOP_GENERATOR"},
        {request_type::BULK_START_GENERATORS, "BULK_START_GENERATORS"},
        {request_type::BULK_STOP_GENERATORS, "BULK_STOP_GENERATORS"},
        {request_type::NONE, "UNKNOWN"} /* Overloaded */
    };

    return (request_types.find(type) == request_types.end()
                ? request_types.at(request_type::NONE)
                : request_types.at(type));
}

std::string to_string(reply_code code)
{
    const static std::unordered_map<reply_code, std::string> reply_codes = {
        {reply_code::OK, "OK"},
        {reply_code::NO_DEVICE, "NO_DEVICE"},
        {reply_code::NO_FILE, "NO_FILE"},
        {reply_code::NO_GENERATOR, "NO_GENERATOR"},
        {reply_code::BAD_INPUT, "BAD_INPUT"},
        {reply_code::ERROR, "ERROR"},
        {reply_code::NONE, "UNKNOWN"}, /* Overloaded */
    };

    return (reply_codes.find(code) == reply_codes.end()
                ? reply_codes.at(reply_code::NONE)
                : reply_codes.at(code));
}

void server::handle_list_generators_request(json& reply)
{
    json jints = json::array();

    for (auto& blkgenerator : blk_generator_stack->block_generators_list())
        jints.emplace_back(make_swagger(*blkgenerator)->toJson());

    reply["code"] = reply_code::OK;
    reply["data"] = jints.dump();
}

void server::handle_create_generator_request(json& request, json& reply)
{
    try {
        json block_generator_json =
            json::parse(request["data"].get<std::string>());

        if (auto id_check =
                config::op_config_validate_id_string(block_generator_json["id"]);
            !id_check)
            throw std::runtime_error(id_check.error().c_str());

        // If user did not specify an id create one for them.
        if (block_generator_json["id"] == core::empty_id_string) {
           block_generator_json["id"] = (core::to_string(core::uuid::random()));
        }

        auto block_generator_model = [&](){
            auto g = BlockGenerator();
            g.fromJson(block_generator_json);
            auto gc = BlockGeneratorConfig();
            gc.fromJson(block_generator_json["config"]);
            g.setConfig(std::make_shared<BlockGeneratorConfig>(gc));
            return g;
        }();

        auto result =
            blk_generator_stack->create_block_generator(*from_swagger(block_generator_model));
        if (!result) { throw std::runtime_error(result.error()); }

        reply["code"] = reply_code::OK;
        reply["data"] = make_swagger(*result.value())->toJson().dump();
    } catch (const std::runtime_error& e) {
        reply["code"] = reply_code::BAD_INPUT;
        reply["error"] = json_error(EINVAL, e.what());
    } catch (const json::exception& e) {
        reply["code"] = reply_code::BAD_INPUT;
        reply["error"] = json_error(e.id, e.what());
    }
}

void server::handle_get_generator_request(json& request, json& reply)
{
    auto blkgenerator = blk_generator_stack->get_block_generator(request["id"]);

    if (blkgenerator == nullptr) {
        reply["code"] = reply_code::NO_GENERATOR;
    } else {
        reply["code"] = reply_code::OK;
        reply["data"] = make_swagger(*blkgenerator)->toJson().dump();
    }
}

void server::handle_delete_generator_request(json& request, json& reply)
{
    blk_generator_stack->delete_block_generator(request["id"]);
    reply["code"] = reply_code::OK;
}

void server::handle_start_generator_request(json& request, json& reply)
{
    auto blkgenerator = blk_generator_stack->get_block_generator(request["id"]);
    if (blkgenerator == nullptr) {
        reply["code"] = reply_code::NO_GENERATOR;
        return;
    }
    blkgenerator->start();
    reply["code"] = reply_code::OK;
}

void server::handle_stop_generator_request(json& request, json& reply)
{
    auto blkgenerator = blk_generator_stack->get_block_generator(request["id"]);

    if (blkgenerator == nullptr) {
        reply["code"] = reply_code::NO_GENERATOR;
        return;
    }

    blkgenerator->stop();
    reply["code"] = reply_code::OK;
}

void server::handle_bulk_start_generators_request(json& request, json& reply)
{
    try {
        BulkStartBlockGeneratorsRequest req;
        BulkStartBlockGeneratorsResponse resp;

        json parsed_request = json::parse(request["data"].get<std::string>());
        req.fromJson(parsed_request);

        for (auto &id : req.getIds()) {
            auto blkgenerator = blk_generator_stack->get_block_generator(id);
            if (blkgenerator == nullptr) {
                reply["code"] = reply_code::NO_GENERATOR;
                auto failed_generator = new BulkStartBlockGeneratorsResponse_failed();
                failed_generator->setId(id);
                failed_generator->setError("Not Found");
                resp.getFailed().push_back(std::shared_ptr<BulkStartBlockGeneratorsResponse_failed>(failed_generator));
                continue;
            }
            blkgenerator->start();
            resp.getSucceeded().push_back(blkgenerator->get_id());
        }

        reply["data"] = resp.toJson().dump();
        reply["code"] = reply_code::OK;
    } catch (const std::runtime_error& e) {
        reply["code"] = reply_code::BAD_INPUT;
        reply["error"] = json_error(EINVAL, e.what());
    } catch (const json::exception& e) {
        reply["code"] = reply_code::BAD_INPUT;
        reply["error"] = json_error(e.id, e.what());
    }
}

void server::handle_bulk_stop_generators_request(json& request, json& reply)
{
    try {
        BulkStopBlockGeneratorsRequest req;
        json parsed_request = json::parse(request["data"].get<std::string>());
        req.fromJson(parsed_request);

        for (auto &id : req.getIds()) {
            auto blkgenerator = blk_generator_stack->get_block_generator(id);
            if (blkgenerator == nullptr) {
                continue;
            }

            blkgenerator->stop();
        }
        reply["code"] = reply_code::OK;
    } catch (const std::runtime_error& e) {
        reply["code"] = reply_code::BAD_INPUT;
        reply["error"] = json_error(EINVAL, e.what());
    } catch (const json::exception& e) {
        reply["code"] = reply_code::BAD_INPUT;
        reply["error"] = json_error(e.id, e.what());
    }
}

void server::handle_list_generator_results_request(json& reply)
{
    json jints = json::array();

    for (auto& blkgenerator : blk_generator_stack->block_generators_list())
        jints.emplace_back(make_swagger(*blkgenerator->get_statistics())->toJson());

    reply["code"] = reply_code::OK;
    reply["data"] = jints.dump();
}

void server::handle_get_generator_result_request(json& request, json& reply)
{
    auto blkgenerator = blk_generator_stack->get_block_generator(request["id"]);

    if (blkgenerator == nullptr) {
        reply["code"] = reply_code::NO_GENERATOR;
    } else {
        reply["code"] = reply_code::OK;
        reply["data"] = make_swagger(*blkgenerator->get_statistics())->toJson().dump();
    }
}

void server::handle_delete_generator_result_request(json& request, json& reply)
{
    auto blkgenerator = blk_generator_stack->get_block_generator(request["id"]);

    if (blkgenerator != nullptr) {
        blkgenerator->clear_statistics();
        reply["code"] = reply_code::OK;
    }
}

reply_msg server::handle_request(const request_block_device_list&)
{
    auto reply = reply_block_devices{};
    for (auto device : blk_device_stack->block_devices_list()) {
        reply.devices.emplace_back(api::to_api_model(*device));
    }
    return reply;
}

reply_msg server::handle_request(const request_block_device& request)
{
    auto reply = reply_block_devices{};
    auto blkdev = blk_device_stack->get_block_device(request.id);

    if (!blkdev) { return to_error(api::error_type::NOT_FOUND); }
    reply.devices.emplace_back(api::to_api_model(*blkdev));

    return reply;
}

reply_msg server::handle_request(const request_block_file_list&)
{
    auto reply = reply_block_files{};
    for (auto blkfile : blk_file_stack->files_list()) {
        reply.files.emplace_back(api::to_api_model(*blkfile));
    }

    return reply;
}

reply_msg server::handle_request(const request_block_file& request)
{
    auto reply = reply_block_files{};
    auto blkfile = blk_file_stack->get_block_file(request.id);

    if (!blkfile) { return to_error(api::error_type::NOT_FOUND); }
    reply.files.emplace_back(api::to_api_model(*blkfile));

    return reply;
}

reply_msg server::handle_request(const request_block_file_add& request)
{
    if (auto id_check =
            config::op_config_validate_id_string(request.source.id);
        !id_check)
        return (to_error(error_type::EAI_ERROR));

    auto block_file_model = api::from_api_model(request.source);
    // If user did not specify an id create one for them.
    if (block_file_model->get_id() == core::empty_id_string) {
        block_file_model->set_id(core::to_string(core::uuid::random()));
    }
    auto result = blk_file_stack->create_block_file(*block_file_model);
    if (!result) { return to_error(error_type::NOT_FOUND); }

    auto reply = reply_block_files{};
    reply.files.emplace_back(api::to_api_model(*result.value()));
    return reply;
}

reply_msg server::handle_request(const request_block_file_del& request)
{
    blk_file_stack->delete_block_file(request.id);
    return reply_ok{};
}

int server::handle_request(const op_event_data* data)
{
    int recv_or_err = 0;
    int send_or_err = 0;
    zmq_msg_t request_msg;
    if (zmq_msg_init(&request_msg) == -1) {
        throw std::runtime_error(zmq_strerror(errno));
    }
    while (
        (recv_or_err = zmq_msg_recv(&request_msg, data->socket, ZMQ_DONTWAIT))
        > 0) {

        std::vector<uint8_t> request_buffer(
            static_cast<uint8_t*>(zmq_msg_data(&request_msg)),
            static_cast<uint8_t*>(zmq_msg_data(&request_msg))
                + zmq_msg_size(&request_msg));

        json request = json::from_cbor(request_buffer);
        request_type type = request.find("type") == request.end()
                                ? request_type::NONE
                                : request["type"].get<request_type>();
        json reply;

        OP_LOG(OP_LOG_TRACE, "Received %s request\n", to_string(type).c_str());

        switch (type) {
        case request_type::LIST_GENERATORS:
            handle_list_generators_request(reply);
            break;
        case request_type::CREATE_GENERATOR:
            handle_create_generator_request(request, reply);
            break;
        case request_type::GET_GENERATOR:
            handle_get_generator_request(request, reply);
            break;
        case request_type::DELETE_GENERATOR:
            handle_delete_generator_request(request, reply);
            break;
        case request_type::START_GENERATOR:
            handle_start_generator_request(request, reply);
            break;
        case request_type::STOP_GENERATOR:
            handle_stop_generator_request(request, reply);
            break;
        case request_type::BULK_START_GENERATORS:
            handle_bulk_start_generators_request(request, reply);
            break;
        case request_type::BULK_STOP_GENERATORS:
            handle_bulk_stop_generators_request(request, reply);
            break;
        case request_type::LIST_GENERATOR_RESULTS:
            handle_list_generator_results_request(reply);
            break;
        case request_type::GET_GENERATOR_RESULT:
            handle_get_generator_result_request(request, reply);
            break;
        case request_type::DELETE_GENERATOR_RESULT:
            handle_delete_generator_result_request(request, reply);
            break;
        default:
            reply["code"] = reply_code::ERROR;
            reply["error"] = json_error(
                ENOSYS, "Request type not implemented in block server");
        }
        std::vector<uint8_t> reply_buffer = json::to_cbor(reply);
        if ((send_or_err = zmq_send(
                 data->socket, reply_buffer.data(), reply_buffer.size(), 0))
            != static_cast<int>(reply_buffer.size())) {
            OP_LOG(OP_LOG_ERROR,
                   "Request reply failed: %s\n",
                   zmq_strerror(errno));
        } else {
            OP_LOG(OP_LOG_TRACE,
                   "Sent %s reply to %s request\n",
                   to_string(reply["code"].get<reply_code>()).c_str(),
                   to_string(type).c_str());
        }
    }
    zmq_msg_close(&request_msg);
    return (((recv_or_err < 0 || send_or_err < 0) && errno == ETERM) ? -1 : 0);
}

static int _handle_rpc_request(const op_event_data* data, void* arg)
{
    auto s = reinterpret_cast<server*>(arg);

    auto reply_errors = 0;
    while (auto request = recv_message(data->socket, ZMQ_DONTWAIT)
                              .and_then(deserialize_request)) {
        auto request_visitor = [&](auto& request) -> reply_msg {
            return (s->handle_request(request));
        };
        auto reply = std::visit(request_visitor, *request);
        if (send_message(data->socket, serialize_reply(reply)) == -1) {
            reply_errors++;
            OP_LOG(
                OP_LOG_ERROR, "Error sending reply: %s\n", zmq_strerror(errno));
            continue;
        }
    }

    return ((reply_errors || errno == ETERM) ? -1 : 0);
}

server::server(void* context, openperf::core::event_loop& loop)
    : m_socket(op_socket_get_server(context, ZMQ_REP, endpoint.data()))
    , blk_device_stack(&device_stack::instance())
    , blk_file_stack(&file_stack::instance())
    , blk_generator_stack(std::make_unique<generator::generator_stack>())
{
    struct op_event_callbacks callbacks = {.on_read = _handle_rpc_request};
    loop.add(m_socket.get(), &callbacks, this);
}

} // namespace openperf::block::api
