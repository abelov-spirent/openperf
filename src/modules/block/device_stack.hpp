#ifndef _OP_BLOCK_DEVICE_HPP_
#define _OP_BLOCK_DEVICE_HPP_

#include "models/device.hpp"
#include <vector>

namespace openperf::block::device {

typedef std::shared_ptr<model::device> device_ptr;
typedef std::map<std::string, device_ptr> device_map;

static const std::string device_dir = "/dev";

class device_stack {
private:
    device_map block_devices;

    void init_device_stack();
    uint64_t get_block_device_size(const std::string id);
    std::string get_block_device_info(const std::string);
    int is_block_device_usable(const std::string id);
    bool is_raw_device(const std::string id);

public:
    device_stack();
    device_ptr get_block_device(std::string id);
    std::vector<device_ptr> block_devices_list();
};

} // namespace openperf::block::device

#endif /* _OP_BLOCK_DEVICE_HPP_ */