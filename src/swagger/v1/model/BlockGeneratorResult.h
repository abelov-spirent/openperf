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
 * BlockGeneratorResult.h
 *
 * Results collected by a running generator
 */

#ifndef BlockGeneratorResult_H_
#define BlockGeneratorResult_H_


#include "ModelBase.h"

#include <string>
#include "BlockGeneratorStats.h"

namespace swagger {
namespace v1 {
namespace model {

/// <summary>
/// Results collected by a running generator
/// </summary>
class  BlockGeneratorResult
    : public ModelBase
{
public:
    BlockGeneratorResult();
    virtual ~BlockGeneratorResult();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// BlockGeneratorResult members

    /// <summary>
    /// Unique generator result identifier
    /// </summary>
    std::string getId() const;
    void setId(std::string value);
        /// <summary>
    /// Block generator identifier that generated this result
    /// </summary>
    std::string getGeneratorId() const;
    void setGeneratorId(std::string value);
    bool generatorIdIsSet() const;
    void unsetGenerator_id();
    /// <summary>
    /// Indicates whether the result is currently being updated
    /// </summary>
    bool isActive() const;
    void setActive(bool value);
        /// <summary>
    /// The ISO8601-formatted date of the last result update
    /// </summary>
    std::string getTimestamp() const;
    void setTimestamp(std::string value);
        /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<BlockGeneratorStats> getRead() const;
    void setRead(std::shared_ptr<BlockGeneratorStats> value);
        /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<BlockGeneratorStats> getWrite() const;
    void setWrite(std::shared_ptr<BlockGeneratorStats> value);
    
protected:
    std::string m_Id;

    std::string m_Generator_id;
    bool m_Generator_idIsSet;
    bool m_Active;

    std::string m_Timestamp;

    std::shared_ptr<BlockGeneratorStats> m_Read;

    std::shared_ptr<BlockGeneratorStats> m_Write;

};

}
}
}

#endif /* BlockGeneratorResult_H_ */
