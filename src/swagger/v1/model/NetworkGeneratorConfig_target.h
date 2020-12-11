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
/*
 * NetworkGeneratorConfig_target.h
 *
 * Network generator connection configuration
 */

#ifndef NetworkGeneratorConfig_target_H_
#define NetworkGeneratorConfig_target_H_


#include "ModelBase.h"

#include <string>

namespace swagger {
namespace v1 {
namespace model {

/// <summary>
/// Network generator connection configuration
/// </summary>
class  NetworkGeneratorConfig_target
    : public ModelBase
{
public:
    NetworkGeneratorConfig_target();
    virtual ~NetworkGeneratorConfig_target();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// NetworkGeneratorConfig_target members

    /// <summary>
    /// Remote host to establish a connection
    /// </summary>
    std::string getHost() const;
    void setHost(std::string value);
        /// <summary>
    /// Port on which client is trying to establish connection
    /// </summary>
    int32_t getPort() const;
    void setPort(int32_t value);
        /// <summary>
    /// Protocol to establish connection between client and host
    /// </summary>
    std::string getProtocol() const;
    void setProtocol(std::string value);
        /// <summary>
    /// Bind client socket to a particular device, specified as interface name
    /// </summary>
    std::string getInterface() const;
    void setInterface(std::string value);
    
protected:
    std::string m_Host;

    int32_t m_Port;

    std::string m_Protocol;

    std::string m_Interface;

};

}
}
}

#endif /* NetworkGeneratorConfig_target_H_ */
