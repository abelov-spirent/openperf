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
 * BulkCreateMemoryGeneratorsRequest.h
 *
 * Parameters for the bulk create operation
 */

#ifndef BulkCreateMemoryGeneratorsRequest_H_
#define BulkCreateMemoryGeneratorsRequest_H_


#include "ModelBase.h"

#include "MemoryGenerator.h"
#include <vector>

namespace swagger {
namespace v1 {
namespace model {

/// <summary>
/// Parameters for the bulk create operation
/// </summary>
class  BulkCreateMemoryGeneratorsRequest
    : public ModelBase
{
public:
    BulkCreateMemoryGeneratorsRequest();
    virtual ~BulkCreateMemoryGeneratorsRequest();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// BulkCreateMemoryGeneratorsRequest members

    /// <summary>
    /// List of memory generators
    /// </summary>
    std::vector<std::shared_ptr<MemoryGenerator>>& getItems();
    
protected:
    std::vector<std::shared_ptr<MemoryGenerator>> m_Items;

};

}
}
}

#endif /* BulkCreateMemoryGeneratorsRequest_H_ */