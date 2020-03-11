#include "block/generator_stack.hpp"

namespace openperf::block::generator {

std::vector<block_generator_ptr> block_generator_stack::block_generators_list()
{
    std::vector<block_generator_ptr> blkgenerators_list;
    for (auto blkgenerator_pair : block_generators) {
        blkgenerators_list.push_back(blkgenerator_pair.second);
    }

    return blkgenerators_list;
}

tl::expected<block_generator_ptr, std::string>
block_generator_stack::create_block_generator(json& block_generator_model)
{
    if (get_block_generator(block_generator_model["id"]))
        return tl::make_unexpected("File " + static_cast<std::string>(block_generator_model["id"])
                                   + " already exists.");

    auto blkgenerator_ptr =
        block_generator_ptr(new block_generator(block_generator_model));
    block_generators.emplace(blkgenerator_ptr->getId(), blkgenerator_ptr);

    return blkgenerator_ptr;
}

block_generator_ptr block_generator_stack::get_block_generator(std::string id)
{
    if (block_generators.count(id)) return block_generators.at(id);
    return NULL;
}

void block_generator_stack::delete_block_generator(std::string id)
{
    block_generators.erase(id);
}

} // namespace openperf::block::generator
