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
 * MemoryGeneratorResult.h
 *
 * Results collected by a running generator
 */

#ifndef MemoryGeneratorResult_H_
#define MemoryGeneratorResult_H_


#include "ModelBase.h"

#include "MemoryGeneratorStats.h"
#include <string>
#include "DynamicResults.h"

namespace swagger {
namespace v1 {
namespace model {

/// <summary>
/// Results collected by a running generator
/// </summary>
class  MemoryGeneratorResult
    : public ModelBase
{
public:
    MemoryGeneratorResult();
    virtual ~MemoryGeneratorResult();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// MemoryGeneratorResult members

    /// <summary>
    /// Unique generator result identifier
    /// </summary>
    std::string getId() const;
    void setId(std::string value);
        /// <summary>
    /// Memory generator identifier that generated this result
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
    /// The ISO8601-formatted start time of the generator
    /// </summary>
    std::string getTimestampFirst() const;
    void setTimestampFirst(std::string value);
        /// <summary>
    /// The ISO8601-formatted date of the last result update
    /// </summary>
    std::string getTimestampLast() const;
    void setTimestampLast(std::string value);
        /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<MemoryGeneratorStats> getRead() const;
    void setRead(std::shared_ptr<MemoryGeneratorStats> value);
        /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<MemoryGeneratorStats> getWrite() const;
    void setWrite(std::shared_ptr<MemoryGeneratorStats> value);
        /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<DynamicResults> getDynamicResults() const;
    void setDynamicResults(std::shared_ptr<DynamicResults> value);
    bool dynamicResultsIsSet() const;
    void unsetDynamic_results();

protected:
    std::string m_Id;

    std::string m_Generator_id;
    bool m_Generator_idIsSet;
    bool m_Active;

    std::string m_Timestamp_first;

    std::string m_Timestamp_last;

    std::shared_ptr<MemoryGeneratorStats> m_Read;

    std::shared_ptr<MemoryGeneratorStats> m_Write;

    std::shared_ptr<DynamicResults> m_Dynamic_results;
    bool m_Dynamic_resultsIsSet;
};

}
}
}

#endif /* MemoryGeneratorResult_H_ */
