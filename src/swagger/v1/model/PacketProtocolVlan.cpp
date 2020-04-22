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


#include "PacketProtocolVlan.h"

namespace swagger {
namespace v1 {
namespace model {

PacketProtocolVlan::PacketProtocolVlan()
{
    m_Drop_eligible = false;
    m_Drop_eligibleIsSet = false;
    m_Ether_type = 0;
    m_Ether_typeIsSet = false;
    m_Id = 0;
    m_IdIsSet = false;
    m_Priority = "";
    m_PriorityIsSet = false;
    
}

PacketProtocolVlan::~PacketProtocolVlan()
{
}

void PacketProtocolVlan::validate()
{
    // TODO: implement validation
}

nlohmann::json PacketProtocolVlan::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    if(m_Drop_eligibleIsSet)
    {
        val["drop_eligible"] = m_Drop_eligible;
    }
    if(m_Ether_typeIsSet)
    {
        val["ether_type"] = m_Ether_type;
    }
    if(m_IdIsSet)
    {
        val["id"] = m_Id;
    }
    if(m_PriorityIsSet)
    {
        val["priority"] = ModelBase::toJson(m_Priority);
    }
    

    return val;
}

void PacketProtocolVlan::fromJson(nlohmann::json& val)
{
    if(val.find("drop_eligible") != val.end())
    {
        setDropEligible(val.at("drop_eligible"));
    }
    if(val.find("ether_type") != val.end())
    {
        setEtherType(val.at("ether_type"));
    }
    if(val.find("id") != val.end())
    {
        setId(val.at("id"));
    }
    if(val.find("priority") != val.end())
    {
        setPriority(val.at("priority"));
        
    }
    
}


bool PacketProtocolVlan::isDropEligible() const
{
    return m_Drop_eligible;
}
void PacketProtocolVlan::setDropEligible(bool value)
{
    m_Drop_eligible = value;
    m_Drop_eligibleIsSet = true;
}
bool PacketProtocolVlan::dropEligibleIsSet() const
{
    return m_Drop_eligibleIsSet;
}
void PacketProtocolVlan::unsetDrop_eligible()
{
    m_Drop_eligibleIsSet = false;
}
int32_t PacketProtocolVlan::getEtherType() const
{
    return m_Ether_type;
}
void PacketProtocolVlan::setEtherType(int32_t value)
{
    m_Ether_type = value;
    m_Ether_typeIsSet = true;
}
bool PacketProtocolVlan::etherTypeIsSet() const
{
    return m_Ether_typeIsSet;
}
void PacketProtocolVlan::unsetEther_type()
{
    m_Ether_typeIsSet = false;
}
int32_t PacketProtocolVlan::getId() const
{
    return m_Id;
}
void PacketProtocolVlan::setId(int32_t value)
{
    m_Id = value;
    m_IdIsSet = true;
}
bool PacketProtocolVlan::idIsSet() const
{
    return m_IdIsSet;
}
void PacketProtocolVlan::unsetId()
{
    m_IdIsSet = false;
}
std::string PacketProtocolVlan::getPriority() const
{
    return m_Priority;
}
void PacketProtocolVlan::setPriority(std::string value)
{
    m_Priority = value;
    m_PriorityIsSet = true;
}
bool PacketProtocolVlan::priorityIsSet() const
{
    return m_PriorityIsSet;
}
void PacketProtocolVlan::unsetPriority()
{
    m_PriorityIsSet = false;
}

}
}
}
