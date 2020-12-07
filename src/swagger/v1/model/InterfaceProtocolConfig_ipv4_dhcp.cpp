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


#include "InterfaceProtocolConfig_ipv4_dhcp.h"

namespace swagger {
namespace v1 {
namespace model {

InterfaceProtocolConfig_ipv4_dhcp::InterfaceProtocolConfig_ipv4_dhcp()
{
    m_Hostname = "";
    m_HostnameIsSet = false;
    m_Client = "";
    m_ClientIsSet = false;
    m_StatusIsSet = false;
    
}

InterfaceProtocolConfig_ipv4_dhcp::~InterfaceProtocolConfig_ipv4_dhcp()
{
}

void InterfaceProtocolConfig_ipv4_dhcp::validate()
{
    // TODO: implement validation
}

nlohmann::json InterfaceProtocolConfig_ipv4_dhcp::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    if(m_HostnameIsSet)
    {
        val["hostname"] = ModelBase::toJson(m_Hostname);
    }
    if(m_ClientIsSet)
    {
        val["client"] = ModelBase::toJson(m_Client);
    }
    if(m_StatusIsSet)
    {
        val["status"] = ModelBase::toJson(m_Status);
    }
    

    return val;
}

void InterfaceProtocolConfig_ipv4_dhcp::fromJson(nlohmann::json& val)
{
    if(val.find("hostname") != val.end())
    {
        setHostname(val.at("hostname"));
        
    }
    if(val.find("client") != val.end())
    {
        setClient(val.at("client"));
        
    }
    if(val.find("status") != val.end())
    {
        if(!val["status"].is_null())
        {
            std::shared_ptr<InterfaceProtocolConfig_ipv4_dhcp_status> newItem(new InterfaceProtocolConfig_ipv4_dhcp_status());
            newItem->fromJson(val["status"]);
            setStatus( newItem );
        }
        
    }
    
}


std::string InterfaceProtocolConfig_ipv4_dhcp::getHostname() const
{
    return m_Hostname;
}
void InterfaceProtocolConfig_ipv4_dhcp::setHostname(std::string value)
{
    m_Hostname = value;
    m_HostnameIsSet = true;
}
bool InterfaceProtocolConfig_ipv4_dhcp::hostnameIsSet() const
{
    return m_HostnameIsSet;
}
void InterfaceProtocolConfig_ipv4_dhcp::unsetHostname()
{
    m_HostnameIsSet = false;
}
std::string InterfaceProtocolConfig_ipv4_dhcp::getClient() const
{
    return m_Client;
}
void InterfaceProtocolConfig_ipv4_dhcp::setClient(std::string value)
{
    m_Client = value;
    m_ClientIsSet = true;
}
bool InterfaceProtocolConfig_ipv4_dhcp::clientIsSet() const
{
    return m_ClientIsSet;
}
void InterfaceProtocolConfig_ipv4_dhcp::unsetClient()
{
    m_ClientIsSet = false;
}
std::shared_ptr<InterfaceProtocolConfig_ipv4_dhcp_status> InterfaceProtocolConfig_ipv4_dhcp::getStatus() const
{
    return m_Status;
}
void InterfaceProtocolConfig_ipv4_dhcp::setStatus(std::shared_ptr<InterfaceProtocolConfig_ipv4_dhcp_status> value)
{
    m_Status = value;
    m_StatusIsSet = true;
}
bool InterfaceProtocolConfig_ipv4_dhcp::statusIsSet() const
{
    return m_StatusIsSet;
}
void InterfaceProtocolConfig_ipv4_dhcp::unsetStatus()
{
    m_StatusIsSet = false;
}

}
}
}

