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


#include "TvlpProfile_network_series.h"

namespace swagger {
namespace v1 {
namespace model {

TvlpProfile_network_series::TvlpProfile_network_series()
{
    m_Length = 0L;
    
}

TvlpProfile_network_series::~TvlpProfile_network_series()
{
}

void TvlpProfile_network_series::validate()
{
    // TODO: implement validation
}

nlohmann::json TvlpProfile_network_series::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["length"] = m_Length;
    val["config"] = ModelBase::toJson(m_Config);
    

    return val;
}

void TvlpProfile_network_series::fromJson(nlohmann::json& val)
{
    setLength(val.at("length"));
    
}


int64_t TvlpProfile_network_series::getLength() const
{
    return m_Length;
}
void TvlpProfile_network_series::setLength(int64_t value)
{
    m_Length = value;
    
}
std::shared_ptr<NetworkGeneratorConfig> TvlpProfile_network_series::getConfig() const
{
    return m_Config;
}
void TvlpProfile_network_series::setConfig(std::shared_ptr<NetworkGeneratorConfig> value)
{
    m_Config = value;
    
}

}
}
}
