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
 * BulkCreateAnalyzersRequest.h
 *
 * Parameters for the bulk create operation
 */

#ifndef BulkCreateAnalyzersRequest_H_
#define BulkCreateAnalyzersRequest_H_


#include "ModelBase.h"

#include "PacketAnalyzer.h"
#include <vector>

namespace swagger {
namespace v1 {
namespace model {

/// <summary>
/// Parameters for the bulk create operation
/// </summary>
class  BulkCreateAnalyzersRequest
    : public ModelBase
{
public:
    BulkCreateAnalyzersRequest();
    virtual ~BulkCreateAnalyzersRequest();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// BulkCreateAnalyzersRequest members

    /// <summary>
    /// List of packet analyzers
    /// </summary>
    std::vector<std::shared_ptr<PacketAnalyzer>>& getItems();
    
protected:
    std::vector<std::shared_ptr<PacketAnalyzer>> m_Items;

};

}
}
}

#endif /* BulkCreateAnalyzersRequest_H_ */
