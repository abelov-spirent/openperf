#ifndef _OP_BLOCK_GENERATOR_HPP_
#define _OP_BLOCK_GENERATOR_HPP_

#include "task.hpp"
#include "virtual_device.hpp"
#include "models/generator.hpp"
#include "models/generator_result.hpp"
#include "utils/worker/worker.hpp"

namespace openperf::block::generator {

using namespace openperf::block::worker;

using block_worker = utils::worker::worker<block_task>;
using block_worker_ptr = std::unique_ptr<block_worker>;
using block_result_ptr = std::shared_ptr<model::block_generator_result>;

class block_generator : public model::block_generator
{
private:
    block_worker_ptr blkworker;
    std::vector<virtual_device_stack*> vdev_stack_list;
    std::shared_ptr<virtual_device> blkdevice;
    task_config_t generate_worker_config(const model::block_generator_config&);
    void update_resource(std::string_view);

public:
    ~block_generator();
    block_generator(const model::block_generator& generator_model,
                    const std::vector<virtual_device_stack*> vdev_stack_list);
    void start();
    void stop();

    void set_config(const model::block_generator_config& value);
    void set_resource_id(std::string_view);
    void set_running(bool value);

    block_result_ptr get_statistics() const;
    void clear_statistics();
};

} // namespace openperf::block::generator

#endif /* _OP_BLOCK_GENERATOR_HPP_ */