#ifndef _LIB_PACKET_PROTOCOL_IPV4_HPP_
#define _LIB_PACKET_PROTOCOL_IPV4_HPP_

/**
 * IPv4 header for the packet header C++ Library
 *
 * This file is automatically generated by the library code generator.
 * Do not edit this file manually.
 **/

#include <type_traits>
#include "packet/type/endian.hpp"
#include "packet/type/enum_flags.hpp"
#include "packet/type/ipv4_address.hpp"

namespace libpacket::protocol {

struct ipv4
{
    static constexpr size_t protocol_field_count = 13;
    static constexpr uint16_t protocol_length = 20;
    static constexpr std::string_view protocol_name = "ipv4";

    enum class field_name
    {
        none,
        version,
        header_length,
        dscp,
        ecn,
        total_length,
        identification,
        flags,
        fragment_offset,
        time_to_live,
        protocol,
        checksum,
        source,
        destination,
    };

    enum class ecn_value
    {
        non_ect = 0x0,
        ect_0 = 0x2,
        ect_1 = 0x1,
        ce = 0x3,
    };

    enum class flags_value
    {
        evil_bit = 0x4,
        dont_fragment = 0x2,
        more_fragments = 0x1,
    };

    type::endian::field<1> version_header_length;
    type::endian::field<1> dscp_ecn;
    type::endian::number<uint16_t> total_length;
    type::endian::field<2> identification;
    type::endian::field<2> flags_fragment_offset;
    type::endian::number<uint8_t> time_to_live;
    type::endian::field<1> protocol;
    type::endian::field<2> checksum;
    type::ipv4_address source;
    type::ipv4_address destination;

    static enum ipv4::field_name get_field_name(std::string_view name) noexcept;
    static const std::type_info& get_field_type(field_name field) noexcept;

    template <typename Value>
    void set_field(enum field_name field, Value value) noexcept;
};

/**
 * IPv4 get functions
 **/

uint8_t get_ipv4_version(const ipv4& header) noexcept;
uint8_t get_ipv4_header_length(const ipv4& header) noexcept;
uint8_t get_ipv4_dscp(const ipv4& header) noexcept;
enum ipv4::ecn_value get_ipv4_ecn(const ipv4& header) noexcept;
uint16_t get_ipv4_total_length(const ipv4& header) noexcept;
uint16_t get_ipv4_identification(const ipv4& header) noexcept;
type::bit_flags<ipv4::flags_value> get_ipv4_flags(const ipv4& header) noexcept;
uint16_t get_ipv4_fragment_offset(const ipv4& header) noexcept;
uint8_t get_ipv4_time_to_live(const ipv4& header) noexcept;
uint8_t get_ipv4_protocol(const ipv4& header) noexcept;
uint16_t get_ipv4_checksum(const ipv4& header) noexcept;
const type::ipv4_address& get_ipv4_source(const ipv4& header) noexcept;
const type::ipv4_address& get_ipv4_destination(const ipv4& header) noexcept;

/**
 * IPv4 set functions
 **/

void set_ipv4_defaults(ipv4& header) noexcept;
void set_ipv4_version(ipv4& header, uint8_t value) noexcept;
void set_ipv4_header_length(ipv4& header, uint8_t value) noexcept;
void set_ipv4_dscp(ipv4& header, uint8_t value) noexcept;
void set_ipv4_ecn(ipv4& header, enum ipv4::ecn_value value) noexcept;
void set_ipv4_total_length(ipv4& header, uint16_t value) noexcept;
void set_ipv4_identification(ipv4& header, uint16_t value) noexcept;
void set_ipv4_flags(ipv4& header, type::bit_flags<ipv4::flags_value> value) noexcept;
void set_ipv4_fragment_offset(ipv4& header, uint16_t value) noexcept;
void set_ipv4_time_to_live(ipv4& header, uint8_t value) noexcept;
void set_ipv4_protocol(ipv4& header, uint8_t value) noexcept;
void set_ipv4_checksum(ipv4& header, uint16_t value) noexcept;
void set_ipv4_source(ipv4& header, const type::ipv4_address& value) noexcept;
void set_ipv4_source(ipv4& header, type::ipv4_address&& value) noexcept;
void set_ipv4_destination(ipv4& header, const type::ipv4_address& value) noexcept;
void set_ipv4_destination(ipv4& header, type::ipv4_address&& value) noexcept;

/**
 * IPv4 generic functions
 **/

template <typename Value>
void ipv4::set_field(enum ipv4::field_name field, Value value) noexcept
{
    switch (field) {
        case ipv4::field_name::version:
            if constexpr (std::is_convertible_v<Value, uint8_t>) {
                set_ipv4_version(*this, value);
            }
            break;
        case ipv4::field_name::header_length:
            if constexpr (std::is_convertible_v<Value, uint8_t>) {
                set_ipv4_header_length(*this, value);
            }
            break;
        case ipv4::field_name::dscp:
            if constexpr (std::is_convertible_v<Value, uint8_t>) {
                set_ipv4_dscp(*this, value);
            }
            break;
        case ipv4::field_name::ecn:
            if constexpr (std::is_convertible_v<Value, uint8_t>) {
                set_ipv4_ecn(*this, static_cast<enum ipv4::ecn_value>(value));
            }
            break;
        case ipv4::field_name::total_length:
            if constexpr (std::is_convertible_v<Value, uint16_t>) {
                set_ipv4_total_length(*this, value);
            }
            break;
        case ipv4::field_name::identification:
            if constexpr (std::is_convertible_v<Value, uint16_t>) {
                set_ipv4_identification(*this, value);
            }
            break;
        case ipv4::field_name::flags:
            if constexpr (std::is_convertible_v<Value, uint16_t>) {
                set_ipv4_flags(*this, static_cast<type::bit_flags<ipv4::flags_value>>(value));
            }
            break;
        case ipv4::field_name::fragment_offset:
            if constexpr (std::is_convertible_v<Value, uint16_t>) {
                set_ipv4_fragment_offset(*this, value);
            }
            break;
        case ipv4::field_name::time_to_live:
            if constexpr (std::is_convertible_v<Value, uint8_t>) {
                set_ipv4_time_to_live(*this, value);
            }
            break;
        case ipv4::field_name::protocol:
            if constexpr (std::is_convertible_v<Value, uint8_t>) {
                set_ipv4_protocol(*this, value);
            }
            break;
        case ipv4::field_name::checksum:
            if constexpr (std::is_convertible_v<Value, uint16_t>) {
                set_ipv4_checksum(*this, value);
            }
            break;
        case ipv4::field_name::source:
            if constexpr (std::is_convertible_v<Value, type::ipv4_address>) {
                set_ipv4_source(*this, value);
            }
            break;
        case ipv4::field_name::destination:
            if constexpr (std::is_convertible_v<Value, type::ipv4_address>) {
                set_ipv4_destination(*this, value);
            }
            break;
        default:
            break; /* no-op */
    }
}

}

declare_libpacket_enum_flags(libpacket::protocol::ipv4::flags_value);

#endif /* _LIB_PACKET_PROTOCOL_IPV4_HPP_ */
