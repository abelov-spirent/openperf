#ifndef _OP_BLOCK_GENERATOR_MODEL_HPP_
#define _OP_BLOCK_GENERATOR_MODEL_HPP_

#include <string>

namespace openperf::block::model {

enum class block_generation_pattern {
    RANDOM,
    SEQUENTIAL,
    REVERSE
};

struct block_generator_config {
    int32_t queue_depth;
    int32_t reads_per_sec;
    int32_t read_size;
    int32_t writes_per_sec;
    int32_t write_size;
    block_generation_pattern pattern;
};

class block_generator
{
public:
    block_generator() = default;
    block_generator(const block_generator &) = default;

    std::string get_id() const;
    void set_id(const std::string& value);
    
    block_generator_config get_config() const;
    void set_config(const block_generator_config& value);
    
    std::string get_resource_id() const;
    void set_resource_id(const std::string& value);
    
    bool is_running() const;
    void set_running(const bool value);
    
protected:
    std::string id;
    block_generator_config config;
    std::string resource_id;
    bool running;
};

} // openperf::block::model

#endif // _OP_BLOCK_GENERATOR_MODEL_HPP_