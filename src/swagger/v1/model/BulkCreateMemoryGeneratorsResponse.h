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
 * BulkCreateMemoryGeneratorsResponse.h
 *
 * 
 */

#ifndef BulkCreateMemoryGeneratorsResponse_H_
#define BulkCreateMemoryGeneratorsResponse_H_


#include "ModelBase.h"

#include "MemoryGenerator.h"
#include <vector>

namespace swagger {
namespace v1 {
namespace model {

/// <summary>
/// 
/// </summary>
class  BulkCreateMemoryGeneratorsResponse
    : public ModelBase
{
public:
    BulkCreateMemoryGeneratorsResponse();
    virtual ~BulkCreateMemoryGeneratorsResponse();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// BulkCreateMemoryGeneratorsResponse members

    /// <summary>
    /// List of created memory generators
    /// </summary>
    std::vector<std::shared_ptr<MemoryGenerator>>& getItems();
    
protected:
    std::vector<std::shared_ptr<MemoryGenerator>> m_Items;

};

}
}
}

#endif /* BulkCreateMemoryGeneratorsResponse_H_ */
