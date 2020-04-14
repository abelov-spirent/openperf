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


#include "BulkStartCpuGeneratorsRequest.h"

namespace swagger {
namespace v1 {
namespace model {

BulkStartCpuGeneratorsRequest::BulkStartCpuGeneratorsRequest()
{
    
}

BulkStartCpuGeneratorsRequest::~BulkStartCpuGeneratorsRequest()
{
}

void BulkStartCpuGeneratorsRequest::validate()
{
    // TODO: implement validation
}

nlohmann::json BulkStartCpuGeneratorsRequest::toJson() const
{
    nlohmann::json val = nlohmann::json::object();

    {
        nlohmann::json jsonArray;
        for( auto& item : m_Ids )
        {
            jsonArray.push_back(ModelBase::toJson(item));
        }
        val["ids"] = jsonArray;
            }
    

    return val;
}

void BulkStartCpuGeneratorsRequest::fromJson(nlohmann::json& val)
{
    {
        m_Ids.clear();
        nlohmann::json jsonArray;
                for( auto& item : val["ids"] )
        {
            m_Ids.push_back(item);
            
        }
    }
    
}


std::vector<std::string>& BulkStartCpuGeneratorsRequest::getIds()
{
    return m_Ids;
}

}
}
}

