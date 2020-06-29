/**
* OpenPerf API
* REST API interface for OpenPerf
*
* OpenAPI spec version: 1
* Contact: support@spirent.com
*
* NOTE: This class is auto generated by the swagger code generator program.
* https://github.com/swagger-api/swagger-codegen.git
* Do not edit the class manually.
*/


#include "PacketIpProtocolCounters.h"

namespace swagger {
namespace v1 {
namespace model {

PacketIpProtocolCounters::PacketIpProtocolCounters()
{
    m_Ipv4 = 0L;
    m_Ipv6 = 0L;
    
}

PacketIpProtocolCounters::~PacketIpProtocolCounters()
{
}

void PacketIpProtocolCounters::validate()
{
    // TODO: implement validation
}

nlohmann::json PacketIpProtocolCounters::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["ipv4"] = m_Ipv4;
    val["ipv6"] = m_Ipv6;
    

    return val;
}

void PacketIpProtocolCounters::fromJson(nlohmann::json& val)
{
    setIpv4(val.at("ipv4"));
    setIpv6(val.at("ipv6"));
    
}


int64_t PacketIpProtocolCounters::getIpv4() const
{
    return m_Ipv4;
}
void PacketIpProtocolCounters::setIpv4(int64_t value)
{
    m_Ipv4 = value;
    
}
int64_t PacketIpProtocolCounters::getIpv6() const
{
    return m_Ipv6;
}
void PacketIpProtocolCounters::setIpv6(int64_t value)
{
    m_Ipv6 = value;
    
}

}
}
}
