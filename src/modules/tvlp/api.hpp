#ifndef _OP_TVLP_API_HPP_
#define _OP_TVLP_API_HPP_

#include <string>
#include <variant>

#include <json.hpp>
#include <zmq.h>
#include <tl/expected.hpp>

#include "models/tvlp_config.hpp"

namespace openperf::tvlp::api {

static constexpr auto endpoint = "inproc://openperf_tvlp";

using tvlp_config_t = model::tvlp_configuration_t;
using tvlp_config_ptr = std::unique_ptr<tvlp_config_t>;

struct message
{};
struct id_message : message
{
    std::string id;
};

namespace request {

namespace tvlp {

struct list : message
{};
struct get : id_message
{};
struct erase : id_message
{};
struct create : message
{
    std::unique_ptr<tvlp_config_t> data;
};
struct stop : id_message
{};
struct start : id_message
{};

} // namespace tvlp
} // namespace request

namespace reply {

struct ok : message
{};
static constexpr size_t err_max_length = 256;
struct error : message
{
    enum type_t {
        NONE = 0,
        NOT_FOUND,
        EXISTS,
        INVALID_ID,
        ZMQ_ERROR,
        BAD_REQUEST_ERROR
    } type = NONE;
    int code = 0;
    char value[err_max_length];
};

namespace tvlp {
struct item : message
{
    tvlp_config_ptr data;
};

struct list : message
{
    std::unique_ptr<std::vector<tvlp_config_t>> data;
};

} // namespace tvlp

} // namespace reply

// Variant types
using api_request = std::variant<request::tvlp::list,
                                 request::tvlp::get,
                                 request::tvlp::erase,
                                 request::tvlp::create,
                                 request::tvlp::start,
                                 request::tvlp::stop>;

using api_reply =
    std::variant<reply::ok, reply::error, reply::tvlp::list, reply::tvlp::item>;

struct serialized_msg
{
    zmq_msg_t type;
    zmq_msg_t data;
};

int send_message(void* socket, serialized_msg&& msg);
tl::expected<serialized_msg, int> recv_message(void* socket, int flags = 0);

serialized_msg serialize(api_request&& request);
serialized_msg serialize(api_reply&& reply);

tl::expected<api_request, int> deserialize_request(const serialized_msg& msg);
tl::expected<api_reply, int> deserialize_reply(const serialized_msg& msg);

} // namespace openperf::tvlp::api

#endif