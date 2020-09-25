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


#include "TvlpProfile.h"

namespace swagger {
namespace v1 {
namespace model {

TvlpProfile::TvlpProfile()
{
    m_MemoryIsSet = false;
    m_BlockIsSet = false;
    m_CpuIsSet = false;
    m_PacketIsSet = false;
    
}

TvlpProfile::~TvlpProfile()
{
}

void TvlpProfile::validate()
{
    // TODO: implement validation
}

nlohmann::json TvlpProfile::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    if(m_MemoryIsSet)
    {
        val["memory"] = ModelBase::toJson(m_Memory);
    }
    if(m_BlockIsSet)
    {
        val["block"] = ModelBase::toJson(m_Block);
    }
    if(m_CpuIsSet)
    {
        val["cpu"] = ModelBase::toJson(m_Cpu);
    }
    if(m_PacketIsSet)
    {
        val["packet"] = ModelBase::toJson(m_Packet);
    }
    

    return val;
}

void TvlpProfile::fromJson(nlohmann::json& val)
{
    if(val.find("memory") != val.end())
    {
        if(!val["memory"].is_null())
        {
            std::shared_ptr<TvlpProfile_memory> newItem(new TvlpProfile_memory());
            newItem->fromJson(val["memory"]);
            setMemory( newItem );
        }
        
    }
    if(val.find("block") != val.end())
    {
        if(!val["block"].is_null())
        {
            std::shared_ptr<TvlpProfile_block> newItem(new TvlpProfile_block());
            newItem->fromJson(val["block"]);
            setBlock( newItem );
        }
        
    }
    if(val.find("cpu") != val.end())
    {
        if(!val["cpu"].is_null())
        {
            std::shared_ptr<TvlpProfile_cpu> newItem(new TvlpProfile_cpu());
            newItem->fromJson(val["cpu"]);
            setCpu( newItem );
        }
        
    }
    if(val.find("packet") != val.end())
    {
        if(!val["packet"].is_null())
        {
            std::shared_ptr<TvlpProfile_packet> newItem(new TvlpProfile_packet());
            newItem->fromJson(val["packet"]);
            setPacket( newItem );
        }
        
    }
    
}


std::shared_ptr<TvlpProfile_memory> TvlpProfile::getMemory() const
{
    return m_Memory;
}
void TvlpProfile::setMemory(std::shared_ptr<TvlpProfile_memory> value)
{
    m_Memory = value;
    m_MemoryIsSet = true;
}
bool TvlpProfile::memoryIsSet() const
{
    return m_MemoryIsSet;
}
void TvlpProfile::unsetMemory()
{
    m_MemoryIsSet = false;
}
std::shared_ptr<TvlpProfile_block> TvlpProfile::getBlock() const
{
    return m_Block;
}
void TvlpProfile::setBlock(std::shared_ptr<TvlpProfile_block> value)
{
    m_Block = value;
    m_BlockIsSet = true;
}
bool TvlpProfile::blockIsSet() const
{
    return m_BlockIsSet;
}
void TvlpProfile::unsetBlock()
{
    m_BlockIsSet = false;
}
std::shared_ptr<TvlpProfile_cpu> TvlpProfile::getCpu() const
{
    return m_Cpu;
}
void TvlpProfile::setCpu(std::shared_ptr<TvlpProfile_cpu> value)
{
    m_Cpu = value;
    m_CpuIsSet = true;
}
bool TvlpProfile::cpuIsSet() const
{
    return m_CpuIsSet;
}
void TvlpProfile::unsetCpu()
{
    m_CpuIsSet = false;
}
std::shared_ptr<TvlpProfile_packet> TvlpProfile::getPacket() const
{
    return m_Packet;
}
void TvlpProfile::setPacket(std::shared_ptr<TvlpProfile_packet> value)
{
    m_Packet = value;
    m_PacketIsSet = true;
}
bool TvlpProfile::packetIsSet() const
{
    return m_PacketIsSet;
}
void TvlpProfile::unsetPacket()
{
    m_PacketIsSet = false;
}

}
}
}

