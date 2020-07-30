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
 * TvlpConfiguration_time.h
 *
 * read only TVLP time parameters
 */

#ifndef TvlpConfiguration_time_H_
#define TvlpConfiguration_time_H_


#include "ModelBase.h"

#include <string>

namespace swagger {
namespace v1 {
namespace model {

/// <summary>
/// read only TVLP time parameters
/// </summary>
class  TvlpConfiguration_time
    : public ModelBase
{
public:
    TvlpConfiguration_time();
    virtual ~TvlpConfiguration_time();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    nlohmann::json toJson() const override;
    void fromJson(nlohmann::json& json) override;

    /////////////////////////////////////////////
    /// TvlpConfiguration_time members

    /// <summary>
    /// The ISO8601-formatted date to start TVLP profile (only available when countdown)
    /// </summary>
    std::string getStart() const;
    void setStart(std::string value);
    bool startIsSet() const;
    void unsetStart();
    /// <summary>
    /// length of the current tvlp profile in ns
    /// </summary>
    int64_t getLength() const;
    void setLength(int64_t value);
        /// <summary>
    /// current offset in ns (only available when running)
    /// </summary>
    int64_t getOffset() const;
    void setOffset(int64_t value);
    bool offsetIsSet() const;
    void unsetOffset();

protected:
    std::string m_Start;
    bool m_StartIsSet;
    int64_t m_Length;

    int64_t m_Offset;
    bool m_OffsetIsSet;
};

}
}
}

#endif /* TvlpConfiguration_time_H_ */
