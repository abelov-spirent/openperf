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
 * TvlpProfile_block_series.h
 *
 * TVLP Profile for Block generators configuration
 */

#ifndef TvlpProfile_block_series_H_
#define TvlpProfile_block_series_H_


#include "ModelBase.h"

#include "BlockGeneratorConfig.h"
#include <string>

namespace swagger {
namespace v1 {
namespace model {

/// <summary>
/// TVLP Profile for Block generators configuration
/// </summary>
class  TvlpProfile_block_series
    : public ModelBase
{
public:
    TvlpProfile_block_series();
    virtual ~TvlpProfile_block_series();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// TvlpProfile_block_series members

    /// <summary>
    /// Unique device or file identifier
    /// </summary>
    std::string getResourceId() const;
    void setResourceId(std::string value);
        /// <summary>
    /// Length of a current operation in ns
    /// </summary>
    int64_t getLength() const;
    void setLength(int64_t value);
        /// <summary>
    /// 
    /// </summary>
    std::shared_ptr<BlockGeneratorConfig> getConfig() const;
    void setConfig(std::shared_ptr<BlockGeneratorConfig> value);
    
protected:
    std::string m_Resource_id;

    int64_t m_Length;

    std::shared_ptr<BlockGeneratorConfig> m_Config;

};

}
}
}

#endif /* TvlpProfile_block_series_H_ */
