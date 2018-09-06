/**
* Inception Core API
* REST API interface to the Inception Core
*
* OpenAPI spec version: 1
* Contact: support@spirent.com
*
* NOTE: This class is auto generated by the swagger code generator program.
* https://github.com/swagger-api/swagger-codegen.git
* Do not edit the class manually.
*/


#include "PortDriver_dpdk.h"

namespace icp {
namespace api {
namespace v1 {
namespace model {

PortDriver_dpdk::PortDriver_dpdk()
{
    m_Driver_name = "";
    m_Pci_address = "";
    m_Min_rx_buffer_size = 0;
    m_Max_rx_packet_length = 0;
    m_Max_rx_queues = 0;
    m_Max_tx_queues = 0;
    m_Max_mac_addresses = 0;
    
}

PortDriver_dpdk::~PortDriver_dpdk()
{
}

void PortDriver_dpdk::validate()
{
    // TODO: implement validation
}

nlohmann::json PortDriver_dpdk::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    val["driver_name"] = ModelBase::toJson(m_Driver_name);
    val["pci_address"] = ModelBase::toJson(m_Pci_address);
    val["min_rx_buffer_size"] = m_Min_rx_buffer_size;
    val["max_rx_packet_length"] = m_Max_rx_packet_length;
    val["max_rx_queues"] = m_Max_rx_queues;
    val["max_tx_queues"] = m_Max_tx_queues;
    val["max_mac_addresses"] = m_Max_mac_addresses;
    {
        nlohmann::json jsonArray;
        for( auto& item : m_Rx_offloads )
        {
            jsonArray.push_back(ModelBase::toJson(item));
        }
        val["rx_offloads"] = jsonArray;
            }
    {
        nlohmann::json jsonArray;
        for( auto& item : m_Tx_offloads )
        {
            jsonArray.push_back(ModelBase::toJson(item));
        }
        val["tx_offloads"] = jsonArray;
            }
    

    return val;
}

void PortDriver_dpdk::fromJson(nlohmann::json& val)
{
    setDriverName(val.at("driver_name"));
    setPciAddress(val.at("pci_address"));
    setMinRxBufferSize(val.at("min_rx_buffer_size"));
    setMaxRxPacketLength(val.at("max_rx_packet_length"));
    setMaxRxQueues(val.at("max_rx_queues"));
    setMaxTxQueues(val.at("max_tx_queues"));
    setMaxMacAddresses(val.at("max_mac_addresses"));
    {
        m_Rx_offloads.clear();
        nlohmann::json jsonArray;
                for( auto& item : val["rx_offloads"] )
        {
            m_Rx_offloads.push_back(item);
            
        }
    }
    {
        m_Tx_offloads.clear();
        nlohmann::json jsonArray;
                for( auto& item : val["tx_offloads"] )
        {
            m_Tx_offloads.push_back(item);
            
        }
    }
    
}


std::string PortDriver_dpdk::getDriverName() const
{
    return m_Driver_name;
}
void PortDriver_dpdk::setDriverName(std::string value)
{
    m_Driver_name = value;
    
}
std::string PortDriver_dpdk::getPciAddress() const
{
    return m_Pci_address;
}
void PortDriver_dpdk::setPciAddress(std::string value)
{
    m_Pci_address = value;
    
}
int32_t PortDriver_dpdk::getMinRxBufferSize() const
{
    return m_Min_rx_buffer_size;
}
void PortDriver_dpdk::setMinRxBufferSize(int32_t value)
{
    m_Min_rx_buffer_size = value;
    
}
int32_t PortDriver_dpdk::getMaxRxPacketLength() const
{
    return m_Max_rx_packet_length;
}
void PortDriver_dpdk::setMaxRxPacketLength(int32_t value)
{
    m_Max_rx_packet_length = value;
    
}
int32_t PortDriver_dpdk::getMaxRxQueues() const
{
    return m_Max_rx_queues;
}
void PortDriver_dpdk::setMaxRxQueues(int32_t value)
{
    m_Max_rx_queues = value;
    
}
int32_t PortDriver_dpdk::getMaxTxQueues() const
{
    return m_Max_tx_queues;
}
void PortDriver_dpdk::setMaxTxQueues(int32_t value)
{
    m_Max_tx_queues = value;
    
}
int32_t PortDriver_dpdk::getMaxMacAddresses() const
{
    return m_Max_mac_addresses;
}
void PortDriver_dpdk::setMaxMacAddresses(int32_t value)
{
    m_Max_mac_addresses = value;
    
}
std::vector<std::string>& PortDriver_dpdk::getRxOffloads()
{
    return m_Rx_offloads;
}
std::vector<std::string>& PortDriver_dpdk::getTxOffloads()
{
    return m_Tx_offloads;
}

}
}
}
}

