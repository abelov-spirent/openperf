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


#include "TvlpConfiguration_time.h"

namespace swagger {
namespace v1 {
namespace model {

TvlpConfiguration_time::TvlpConfiguration_time()
{
    m_Start = "";
    m_StartIsSet = false;
    m_Length = 0L;
    m_Offset = 0L;
    m_OffsetIsSet = false;
    
}

TvlpConfiguration_time::~TvlpConfiguration_time()
{
}

void TvlpConfiguration_time::validate()
{
    // TODO: implement validation
}

nlohmann::json TvlpConfiguration_time::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    if(m_StartIsSet)
    {
        val["start"] = ModelBase::toJson(m_Start);
    }
    val["length"] = m_Length;
    if(m_OffsetIsSet)
    {
        val["offset"] = m_Offset;
    }
    

    return val;
}

void TvlpConfiguration_time::fromJson(nlohmann::json& val)
{
    if(val.find("start") != val.end())
    {
        setStart(val.at("start"));
        
    }
    setLength(val.at("length"));
    if(val.find("offset") != val.end())
    {
        setOffset(val.at("offset"));
    }
    
}


std::string TvlpConfiguration_time::getStart() const
{
    return m_Start;
}
void TvlpConfiguration_time::setStart(std::string value)
{
    m_Start = value;
    m_StartIsSet = true;
}
bool TvlpConfiguration_time::startIsSet() const
{
    return m_StartIsSet;
}
void TvlpConfiguration_time::unsetStart()
{
    m_StartIsSet = false;
}
int64_t TvlpConfiguration_time::getLength() const
{
    return m_Length;
}
void TvlpConfiguration_time::setLength(int64_t value)
{
    m_Length = value;
    
}
int64_t TvlpConfiguration_time::getOffset() const
{
    return m_Offset;
}
void TvlpConfiguration_time::setOffset(int64_t value)
{
    m_Offset = value;
    m_OffsetIsSet = true;
}
bool TvlpConfiguration_time::offsetIsSet() const
{
    return m_OffsetIsSet;
}
void TvlpConfiguration_time::unsetOffset()
{
    m_OffsetIsSet = false;
}

}
}
}

