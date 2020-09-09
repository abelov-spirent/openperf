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


#include "MemoryGeneratorConfig.h"

namespace swagger {
namespace v1 {
namespace model {

MemoryGeneratorConfig::MemoryGeneratorConfig()
{
    m_Buffer_size = 0L;
    m_Reads_per_sec = 0;
    m_Read_size = 0;
    m_Read_threads = 0;
    m_Writes_per_sec = 0;
    m_Write_size = 0;
    m_Write_threads = 0;
    m_Pattern = "";
    
}

MemoryGeneratorConfig::~MemoryGeneratorConfig()
{
}

void MemoryGeneratorConfig::validate()
{
    // TODO: implement validation
}

nlohmann::json MemoryGeneratorConfig::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["buffer_size"] = m_Buffer_size;
    val["reads_per_sec"] = m_Reads_per_sec;
    val["read_size"] = m_Read_size;
    val["read_threads"] = m_Read_threads;
    val["writes_per_sec"] = m_Writes_per_sec;
    val["write_size"] = m_Write_size;
    val["write_threads"] = m_Write_threads;
    val["pattern"] = ModelBase::toJson(m_Pattern);
    

    return val;
}

void MemoryGeneratorConfig::fromJson(nlohmann::json& val)
{
    setBufferSize(val.at("buffer_size"));
    setReadsPerSec(val.at("reads_per_sec"));
    setReadSize(val.at("read_size"));
    setReadThreads(val.at("read_threads"));
    setWritesPerSec(val.at("writes_per_sec"));
    setWriteSize(val.at("write_size"));
    setWriteThreads(val.at("write_threads"));
    setPattern(val.at("pattern"));
    
}


int64_t MemoryGeneratorConfig::getBufferSize() const
{
    return m_Buffer_size;
}
void MemoryGeneratorConfig::setBufferSize(int64_t value)
{
    m_Buffer_size = value;
    
}
int32_t MemoryGeneratorConfig::getReadsPerSec() const
{
    return m_Reads_per_sec;
}
void MemoryGeneratorConfig::setReadsPerSec(int32_t value)
{
    m_Reads_per_sec = value;
    
}
int32_t MemoryGeneratorConfig::getReadSize() const
{
    return m_Read_size;
}
void MemoryGeneratorConfig::setReadSize(int32_t value)
{
    m_Read_size = value;
    
}
int32_t MemoryGeneratorConfig::getReadThreads() const
{
    return m_Read_threads;
}
void MemoryGeneratorConfig::setReadThreads(int32_t value)
{
    m_Read_threads = value;
    
}
int32_t MemoryGeneratorConfig::getWritesPerSec() const
{
    return m_Writes_per_sec;
}
void MemoryGeneratorConfig::setWritesPerSec(int32_t value)
{
    m_Writes_per_sec = value;
    
}
int32_t MemoryGeneratorConfig::getWriteSize() const
{
    return m_Write_size;
}
void MemoryGeneratorConfig::setWriteSize(int32_t value)
{
    m_Write_size = value;
    
}
int32_t MemoryGeneratorConfig::getWriteThreads() const
{
    return m_Write_threads;
}
void MemoryGeneratorConfig::setWriteThreads(int32_t value)
{
    m_Write_threads = value;
    
}
std::string MemoryGeneratorConfig::getPattern() const
{
    return m_Pattern;
}
void MemoryGeneratorConfig::setPattern(std::string value)
{
    m_Pattern = value;
    
}

}
}
}

