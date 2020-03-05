#include <zmq.h>

#include "block/api.hpp"
#include "block/server.hpp"
#include "block/device.hpp"
#include "config/op_config_utils.hpp"
#include "swagger/v1/model/BlockFile.h"

namespace openperf::block::api {

using namespace swagger::v1::model;

std::string to_string(request_type type)
{
    const static std::unordered_map<request_type, std::string> request_types = {
        {request_type::LIST_DEVICES, "LIST_INTERFACES"},
        {request_type::GET_DEVICE, "GET_DEVICE"},
        {request_type::LIST_FILES, "LIST_FILES"},
        {request_type::CREATE_FILE, "CREATE_FILE"},
        {request_type::GET_FILE, "GET_FILE"},
        {request_type::DELETE_FILE, "DELETE_FILE"},
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
        {reply_code::BAD_INPUT, "BAD_INPUT"},
        {reply_code::ERROR, "ERROR"},
        {reply_code::NONE, "UNKNOWN"}, /* Overloaded */
    };

    return (reply_codes.find(code) == reply_codes.end()
                ? reply_codes.at(reply_code::NONE)
                : reply_codes.at(code));
}

void server::handle_list_devices_request(json& reply)
{
    json jints = json::array();

    for (const auto & device : device::block_devices_list()) {
        jints.emplace_back(device->toJson());
    }

    reply["code"] = reply_code::OK;
    reply["data"] = jints.dump();
}

void server::handle_get_device_request(json& request, json& reply)
{
    auto blkdev = device::get_block_device(request["id"]);
    if (blkdev) std::cout << blkdev.get() << '\n'; else std::cout << "(null)\n";
    if (blkdev) {
        reply["code"] = reply_code::OK;
        reply["data"] = blkdev->toJson().dump();
    } else {
        reply["code"] = reply_code::NO_DEVICE;
    }
}

void server::handle_list_block_files_request(json& reply)
{
    json jints = json::array();

    for (auto & blkfile : blk_file_stack->block_files_list())
        jints.emplace_back(blkfile->toJson());

    reply["code"] = reply_code::OK;
    reply["data"] = jints.dump();
}

void server::handle_create_block_file_request(json& request, json& reply)
{
    try {
        auto block_file_model = json::parse(request["data"].get<std::string>());
        BlockFile block_file;
        block_file.fromJson(block_file_model);
        
        if (auto id_check = config::op_config_validate_id_string(block_file.getId()); !id_check)
            throw std::runtime_error(id_check.error().c_str());
        
        // If user did not specify an id create one for them.
        if (block_file.getId() == core::empty_id_string) {
            block_file.setId(core::to_string(core::uuid::random()));
        }

        auto result = blk_file_stack->create_block_file(block_file);
        if (!result) { throw std::runtime_error(result.error()); }

        reply["code"] = reply_code::OK;
        reply["data"] = result.value()->toJson().dump();
   } catch (const std::runtime_error& e) {
        reply["code"] = reply_code::BAD_INPUT;
        reply["error"] = json_error(EINVAL, e.what());
    } catch (const json::exception& e) {
        reply["code"] = reply_code::BAD_INPUT;
        reply["error"] = json_error(e.id, e.what());
    }
}

void server::handle_get_block_file_request(json& request, json& reply)
{
    auto blkfile = blk_file_stack->get_block_file(request["id"]);

    if (blkfile == NULL) {
        reply["code"] = reply_code::NO_FILE;
    } else {
        reply["code"] = reply_code::OK;
        reply["data"] = blkfile->toJson().dump();
    }
}

void server::handle_delete_block_file_request(json& request, json& reply)
{
    blk_file_stack->delete_block_file(request["id"]);
    reply["code"] = reply_code::OK;
}

void server::handle_list_generators_request(json& reply)
{
    json jints = json::array();

    for (auto & blkgenerator : blk_generator_stack->block_generators_list())
        jints.emplace_back(blkgenerator->toJson());

    reply["code"] = reply_code::OK;
    reply["data"] = jints.dump();
}

void server::handle_create_generator_request(json& request, json& reply)
{
    try {
        auto block_generator_model = json::parse(request["data"].get<std::string>());
        BlockGenerator block_generator;
        block_generator.fromJson(block_generator_model);
        
        if (auto id_check = config::op_config_validate_id_string(block_generator.getId()); !id_check)
            throw std::runtime_error(id_check.error().c_str());
        
        // If user did not specify an id create one for them.
        if (block_generator.getId() == core::empty_id_string) {
            block_generator.setId(core::to_string(core::uuid::random()));
        }

        auto result = blk_generator_stack->create_block_generator(block_generator);
        if (!result) { throw std::runtime_error(result.error()); }

        reply["code"] = reply_code::OK;
        reply["data"] = result.value()->toJson().dump();
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

    if (blkgenerator == NULL) {
        reply["code"] = reply_code::NO_FILE;
    } else {
        reply["code"] = reply_code::OK;
        reply["data"] = blkgenerator->toJson().dump();
    }
}

void server::handle_delete_generator_request(json& request, json& reply)
{
    blk_generator_stack->delete_block_generator(request["id"]);
    reply["code"] = reply_code::OK;
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

        OP_LOG(
                OP_LOG_TRACE, "Received %s request\n", to_string(type).c_str());
        
        switch (type) {
        case request_type::LIST_DEVICES:
            handle_list_devices_request(reply);
            break;
        case request_type::GET_DEVICE:
            handle_get_device_request(request, reply);
            break;
        case request_type::LIST_FILES:
            handle_list_block_files_request(reply);
            break;
        case request_type::CREATE_FILE:
            handle_create_block_file_request(request, reply);
            break;
        case request_type::GET_FILE:
            handle_get_block_file_request(request, reply);
            break;
        case request_type::DELETE_FILE:
            handle_delete_block_file_request(request, reply);
            break;
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
        default:
            reply["code"] = reply_code::ERROR;
            reply["error"] = json_error(
                ENOSYS,
                "Request type not implemented in block server");
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
    return s->handle_request(data);
}

server::server(void* context, openperf::core::event_loop& loop)
    : m_socket(op_socket_get_server(context, ZMQ_REP, endpoint.data()))
    , blk_file_stack(std::make_unique<file::block_file_stack>())
    , blk_generator_stack(std::make_unique<generator::block_generator_stack>())
{
    struct op_event_callbacks callbacks = {.on_read = _handle_rpc_request};
    loop.add(m_socket.get(), &callbacks, this);
}

} // namespace openperf::block::api
