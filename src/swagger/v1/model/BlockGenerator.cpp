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


#include "BlockGenerator.h"

namespace swagger {
namespace v1 {
namespace model {

BlockGenerator::BlockGenerator()
{
    m_Id = "";
    m_Resource_id = "";
    m_Running = false;
    
}

BlockGenerator::~BlockGenerator()
{
}

void BlockGenerator::validate()
{
    // TODO: implement validation
}

nlohmann::json BlockGenerator::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["id"] = ModelBase::toJson(m_Id);
    val["config"] = ModelBase::toJson(m_Config);
    val["resource_id"] = ModelBase::toJson(m_Resource_id);
    val["running"] = m_Running;
    

    return val;
}

void BlockGenerator::fromJson(nlohmann::json& val)
{
    setId(val.at("id"));
    setResourceId(val.at("resource_id"));
    setRunning(val.at("running"));
    
}


std::string BlockGenerator::getId() const
{
    return m_Id;
}
void BlockGenerator::setId(std::string value)
{
    m_Id = value;
    
}
std::shared_ptr<BlockGeneratorConfig> BlockGenerator::getConfig() const
{
    return m_Config;
}
void BlockGenerator::setConfig(std::shared_ptr<BlockGeneratorConfig> value)
{
    m_Config = value;
    
}
std::string BlockGenerator::getResourceId() const
{
    return m_Resource_id;
}
void BlockGenerator::setResourceId(std::string value)
{
    m_Resource_id = value;
    
}
bool BlockGenerator::isRunning() const
{
    return m_Running;
}
void BlockGenerator::setRunning(bool value)
{
    m_Running = value;
    
}

}
}
}

