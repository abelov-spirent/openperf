#include <memory>

#include <json.hpp>
#include <zmq.h>

#include "api/api_route_handler.hpp"
#include "config/op_config_utils.hpp"
#include "core/op_core.h"
#include "block/api.hpp"

namespace opneperf::block {

using namespace Pistache;
using json = nlohmann::json;
namespace api = openperf::block::api;

class handler : public openperf::api::route::handler::registrar<handler>
{
    std::unique_ptr<void, op_socket_deleter> m_socket;

public:
    handler(void* context, Rest::Router& router);

    void list_devices(const Rest::Request& request,
                            Http::ResponseWriter response);

    void get_device(const Rest::Request& request,
                            Http::ResponseWriter response);

    void list_files(const Rest::Request& request,
                            Http::ResponseWriter response);

    void create_file(const Rest::Request& request,
                            Http::ResponseWriter response);

    void get_file(const Rest::Request& request,
                            Http::ResponseWriter response);

    void delete_file(const Rest::Request& request,
                            Http::ResponseWriter response);

};

json submit_request(void* socket, json& request)
{
    auto type = request["type"].get<api::request_type>();
    switch (type) {
    case api::request_type::LIST_DEVICES:
        OP_LOG(OP_LOG_TRACE,
               "Sending %s request for interface %s\n",
               to_string(type).c_str(),
               request["id"].get<std::string>().c_str());
        break;
    default:
        OP_LOG(OP_LOG_TRACE, "Sending %s request\n", to_string(type).c_str());
    }

    std::vector<uint8_t> request_buffer = json::to_cbor(request);
    zmq_msg_t reply_msg;
    if (zmq_msg_init(&reply_msg) == -1
        || zmq_send(socket, request_buffer.data(), request_buffer.size(), 0)
               != static_cast<int>(request_buffer.size())
        || zmq_msg_recv(&reply_msg, socket, 0) == -1) {
        return {{"code", api::reply_code::ERROR},
                {"error", api::json_error(errno, zmq_strerror(errno))}};
    }

    OP_LOG(OP_LOG_TRACE, "Received %s reply\n", to_string(type).c_str());

    std::vector<uint8_t> reply_buffer(
        static_cast<uint8_t*>(zmq_msg_data(&reply_msg)),
        static_cast<uint8_t*>(zmq_msg_data(&reply_msg))
            + zmq_msg_size(&reply_msg));

    zmq_msg_close(&reply_msg);

    return json::from_cbor(reply_buffer);
}

handler::handler(void* context, Rest::Router& router)
    : m_socket(op_socket_get_client(
        context, ZMQ_REQ, api::endpoint.data()))
{
    /* Time counter handlers */
    Rest::Routes::Get(router,
                      "/block-devices",
                      Rest::Routes::bind(&handler::list_devices, this));
    Rest::Routes::Get(router,
                      "/block-devices/:id",
                      Rest::Routes::bind(&handler::get_device, this));
    Rest::Routes::Get(router,
                      "/block-files",
                      Rest::Routes::bind(&handler::list_files, this));
    Rest::Routes::Post(router,
                      "/block-files",
                      Rest::Routes::bind(&handler::create_file, this));
    Rest::Routes::Get(router,
                      "/block-files/:id",
                      Rest::Routes::bind(&handler::get_file, this));
    Rest::Routes::Delete(router,
                      "/block-files/:id",
                      Rest::Routes::bind(&handler::delete_file, this));
}

void handler::list_devices(const Rest::Request&,
                                 Http::ResponseWriter response)
{
    json api_request = {{"type", api::request_type::LIST_DEVICES}};

    json api_reply = submit_request(m_socket.get(), api_request);

    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
    if (api_reply["code"].get<api::reply_code>() == api::reply_code::OK) {
        response.send(Http::Code::Ok, api_reply["data"].get<std::string>());
    } else {
        response.send(Http::Code::Internal_Server_Error,
                      api_reply["error"].get<std::string>());
    }
}

void handler::get_device(const Rest::Request& request,
                                 Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = openperf::config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Not_Found, res.error());
        return;
    }

    json api_request = {{"type", api::request_type::GET_DEVICE}, {"id", id}};

    json api_reply = submit_request(m_socket.get(), api_request);

    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
    if (api_reply["code"].get<api::reply_code>() == api::reply_code::OK) {
        response.send(Http::Code::Ok, api_reply["data"].get<std::string>());
    } else {
        response.send(Http::Code::Internal_Server_Error,
                      api_reply["error"].get<std::string>());
    }
}


void handler::list_files(const Rest::Request&,
                                 Http::ResponseWriter response)
{
    json api_request = {{"type", api::request_type::LIST_FILES}};

    json api_reply = submit_request(m_socket.get(), api_request);

    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
    if (api_reply["code"].get<api::reply_code>() == api::reply_code::OK) {
        response.send(Http::Code::Ok, api_reply["data"].get<std::string>());
    } else {
        response.send(Http::Code::Internal_Server_Error,
                      api_reply["error"].get<std::string>());
    }
}

void handler::create_file(const Rest::Request& request,
                                 Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = openperf::config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Not_Found, res.error());
        return;
    }

    json api_request = {{"type", api::request_type::CREATE_FILE}, {"id", id}};

    json api_reply = submit_request(m_socket.get(), api_request);

    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
    if (api_reply["code"].get<api::reply_code>() == api::reply_code::OK) {
        response.send(Http::Code::Ok, api_reply["data"].get<std::string>());
    } else {
        response.send(Http::Code::Internal_Server_Error,
                      api_reply["error"].get<std::string>());
    }
}

void handler::get_file(const Rest::Request& request,
                                 Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = openperf::config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Not_Found, res.error());
        return;
    }

    json api_request = {{"type", api::request_type::GET_FILE}, {"id", id}};

    json api_reply = submit_request(m_socket.get(), api_request);

    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
    if (api_reply["code"].get<api::reply_code>() == api::reply_code::OK) {
        response.send(Http::Code::Ok, api_reply["data"].get<std::string>());
    } else {
        response.send(Http::Code::Internal_Server_Error,
                      api_reply["error"].get<std::string>());
    }
}

void handler::delete_file(const Rest::Request& request,
                                 Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = openperf::config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Not_Found, res.error());
        return;
    }

    json api_request = {{"type", api::request_type::DELETE_FILE}, {"id", id}};

    json api_reply = submit_request(m_socket.get(), api_request);

    response.headers().add<Http::Header::ContentType>(MIME(Application, Json));
    if (api_reply["code"].get<api::reply_code>() == api::reply_code::OK) {
        response.send(Http::Code::Ok, api_reply["data"].get<std::string>());
    } else {
        response.send(Http::Code::Internal_Server_Error,
                      api_reply["error"].get<std::string>());
    }
}

} // namespace opneperf::block
