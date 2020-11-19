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


#include "NetworkServer.h"

namespace swagger {
namespace v1 {
namespace model {

NetworkServer::NetworkServer()
{
    m_Id = "";
    m_Port = 0L;
    m_Protocol = "";
    
}

NetworkServer::~NetworkServer()
{
}

void NetworkServer::validate()
{
    // TODO: implement validation
}

nlohmann::json NetworkServer::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["id"] = ModelBase::toJson(m_Id);
    val["port"] = m_Port;
    val["protocol"] = ModelBase::toJson(m_Protocol);
    

    return val;
}

void NetworkServer::fromJson(nlohmann::json& val)
{
    setId(val.at("id"));
    setPort(val.at("port"));
    setProtocol(val.at("protocol"));
    
}


std::string NetworkServer::getId() const
{
    return m_Id;
}
void NetworkServer::setId(std::string value)
{
    m_Id = value;
    
}
int64_t NetworkServer::getPort() const
{
    return m_Port;
}
void NetworkServer::setPort(int64_t value)
{
    m_Port = value;
    
}
std::string NetworkServer::getProtocol() const
{
    return m_Protocol;
}
void NetworkServer::setProtocol(std::string value)
{
    m_Protocol = value;
    
}

}
}
}

