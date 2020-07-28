#include "api.hpp"
#include "swagger_converters.hpp"

#include "framework/core/op_core.h"
#include "framework/config/op_config_utils.hpp"
#include "modules/api/api_route_handler.hpp"

namespace openperf::memory::api {

namespace model = ::swagger::v1::model;
using json = nlohmann::json;
using namespace Pistache;

std::string json_error(std::string_view msg)
{
    return json{"error", msg}.dump();
}

void response_error(Http::ResponseWriter& rsp, reply::error error)
{
    switch (error.type) {
    case reply::error::NOT_FOUND:
        rsp.send(Http::Code::Not_Found);
        break;
    case reply::error::EXISTS:
        rsp.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        rsp.send(Http::Code::Bad_Request,
                 json_error("Item with such ID already existst"));
        break;
    case reply::error::INVALID_ID:
        rsp.headers().add<Http::Header::ContentType>(MIME(Application, Json));
        rsp.send(Http::Code::Bad_Request, json_error("Invalid ID format"));
        break;
    default:
        rsp.send(Http::Code::Internal_Server_Error);
    }
}

class handler : public openperf::api::route::handler::registrar<handler>
{
private:
    std::unique_ptr<void, op_socket_deleter> socket;

private:
    api::api_reply submit_request(api::api_request&&);

public:
    handler(void* context, Rest::Router&);

    // TVLP configuration actions
    void list_tvlp(const Rest::Request&, Http::ResponseWriter);
    void create_tvlp(const Rest::Request&, Http::ResponseWriter);
    void get_tvlp(const Rest::Request&, Http::ResponseWriter);
    void delete_tvlp(const Rest::Request&, Http::ResponseWriter);
    void start_tvlp(const Rest::Request&, Http::ResponseWriter);
    void stop_tvlp(const Rest::Request&, Http::ResponseWriter);

    // TVLP statistics actions
    void list_results(const Rest::Request&, Http::ResponseWriter);
    void get_result(const Rest::Request&, Http::ResponseWriter);
    void delete_result(const Rest::Request&, Http::ResponseWriter);
    s
};

handler::handler(void* context, Rest::Router& router)
    : socket(
        op_socket_get_client(context, ZMQ_REQ, openperf::memory::api::endpoint))
{
    Rest::Routes::Get(
        router, "/tvlp", Rest::Routes::bind(&handler::list_tvlp, this));
    Rest::Routes::Post(
        router, "/tvlp", Rest::Routes::bind(&handler::create_tvlp, this));
    Rest::Routes::Get(
        router, "/tvlp/:id", Rest::Routes::bind(&handler::get_tvlp, this));
    Rest::Routes::Delete(
        router, "/tvlp/:id", Rest::Routes::bind(&handler::delete_tvlp, this));

    Rest::Routes::Post(router,
                       "/tvlp/:id/start",
                       Rest::Routes::bind(&handler::start_tvlp, this));
    Rest::Routes::Post(router,
                       "/tvlp/:id/stop",
                       Rest::Routes::bind(&handler::stop_tvlp, this));

    Rest::Routes::Get(router,
                      "/tvlp-results",
                      Rest::Routes::bind(&handler::list_results, this));
    Rest::Routes::Get(router,
                      "/tvlp-results/:id",
                      Rest::Routes::bind(&handler::get_result, this));
    Rest::Routes::Delete(router,
                         "/tvlp-results/:id",
                         Rest::Routes::bind(&handler::delete_result, this));
}

void handler::list_tvlp(const Rest::Request&, Http::ResponseWriter response)
{
    auto api_reply = submit_request(request::tvlp::list{});

    if (auto list = std::get_if<reply::tvlp::list>(&api_reply)) {
        auto array = json::array();
        std::transform(
            list->data->begin(),
            list->data->end(),
            std::back_inserter(array),
            [](const auto& item) -> json { return to_swagger(item).toJson(); });

        response.headers().add<Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Http::Code::Ok, array.dump());
        return;
    }

    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::Internal_Server_Error);
}

void handler::create_tvlp(const Rest::Request& request,
                          Http::ResponseWriter response)
{
    auto model = json::parse(request.body()).get<model::TvlpConfiguration>();

    request::tvlp::create_data data{
        .id = model.getId(), .is_running = model.isRunning(), .config = [&]() {
            return from_swagger(*model.getConfig());
        }()};
    auto api_reply = submit_request(request::tvlp::create{
        std::make_unique<request::tvlp::create_data>(std::move(data))});

    if (auto item = std::get_if<reply::tvlp::item>(&api_reply)) {
        response.headers().add<Http::Header::ContentType>(
            MIME(Application, Json));
        response.headers().add<Http::Header::Location>("/tvlp/"
                                                       + item->data->id);
        response.send(Http::Code::Created,
                      to_swagger(*item->data).toJson().dump());
        return;
    }

    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::Internal_Server_Error);
}

void handler::get_tvlp(const Rest::Request& request,
                       Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Bad_Request, res.error());
        return;
    }

    auto api_reply = submit_request(request::tvlp::get{{.id = id}});

    if (auto r = std::get_if<reply::tvlp::item>(&api_reply)) {
        response.headers().add<Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Http::Code::Ok, to_swagger(*r->data).toJson().dump());
        return;
    }

    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::Internal_Server_Error);
}

void handler::delete_tvlp(const Rest::Request& request,
                          Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Bad_Request, res.error());
        return;
    }

    auto api_reply = submit_request(request::tvlp::erase{{.id = id}});
    if (auto ok = std::get_if<reply::ok>(&api_reply)) {
        response.send(Http::Code::No_Content);
        return;
    }

    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::Internal_Server_Error);
}

void handler::start_tvlp(const Rest::Request& request,
                         Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Bad_Request, res.error());
        return;
    }

    auto api_reply = submit_request(request::tvlp::start{{.id = id}});
    if (auto item = std::get_if<reply::statistic::item>(&api_reply)) {
        auto model = to_swagger(*item->data);
        response.headers().add<Http::Header::ContentType>(
            MIME(Application, Json));
        response.headers().add<Http::Header::Location>("/memory-tvlp-results/"
                                                       + model.getId());
        response.send(Http::Code::Created, model.toJson().dump());
        return;
    }

    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::Internal_Server_Error);
}

void handler::stop_tvlp(const Rest::Request& request,
                        Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Bad_Request, res.error());
        return;
    }

    auto api_reply = submit_request(request::tvlp::stop{{.id = id}});
    if (auto ok = std::get_if<reply::ok>(&api_reply)) {
        response.send(Http::Code::No_Content);
        return;
    }

    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::Internal_Server_Error);
}

// Memory tvlp results
void handler::list_results(const Rest::Request&, Http::ResponseWriter response)
{
    auto api_reply = submit_request(request::statistic::list{});

    if (auto list = std::get_if<reply::statistic::list>(&api_reply)) {
        auto array = json::array();
        std::transform(
            list->data->begin(),
            list->data->end(),
            std::back_inserter(array),
            [](const auto& item) -> json { return to_swagger(item).toJson(); });

        response.headers().add<Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Http::Code::Ok, array.dump());
        return;
    }

    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::Internal_Server_Error);
}

void handler::get_result(const Rest::Request& request,
                         Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Bad_Request, res.error());
        return;
    }

    auto api_reply = submit_request(request::statistic::get{{.id = id}});
    if (auto item = std::get_if<reply::statistic::item>(&api_reply)) {
        response.headers().add<Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Http::Code::Ok, to_swagger(*item->data).toJson().dump());
        return;
    }

    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::Internal_Server_Error);
}

void handler::delete_result(const Rest::Request& request,
                            Http::ResponseWriter response)
{
    auto id = request.param(":id").as<std::string>();
    if (auto res = config::op_config_validate_id_string(id); !res) {
        response.send(Http::Code::Bad_Request, res.error());
        return;
    }

    auto api_reply = submit_request(request::statistic::erase{{.id = id}});
    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::No_Content);
}

void handler::get_info(const Rest::Request&, Http::ResponseWriter response)
{
    auto api_reply = submit_request(request::info{});

    if (auto info = std::get_if<reply::info>(&api_reply)) {
        response.headers().add<Http::Header::ContentType>(
            MIME(Application, Json));
        response.send(Http::Code::Ok, to_swagger(*info).toJson().dump());
        return;
    }

    if (auto error = std::get_if<reply::error>(&api_reply)) {
        response_error(response, *error);
        return;
    }

    response.send(Http::Code::Internal_Server_Error);
}

// Methods : private
api::api_reply handler::submit_request(api::api_request&& request)
{
    if (auto error = api::send_message(
            socket.get(), api::serialize(std::forward<api_request>(request)));
        error != 0) {
        return api::reply::error{.type = api::reply::error::ZMQ_ERROR,
                                 .value = error};
    }

    auto reply =
        api::recv_message(socket.get()).and_then(api::deserialize_reply);
    if (!reply) {
        return api::reply::error{.type = api::reply::error::ZMQ_ERROR,
                                 .value = reply.error()};
    }

    return std::move(*reply);
}

} // namespace openperf::memory::api
