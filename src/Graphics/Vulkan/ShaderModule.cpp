#include "./ShaderModule.hpp"

namespace Dynamo::Graphics::Vulkan {
    ShaderModule::ShaderModule(Device &device,
                               const std::string filename,
                               vk::ShaderStageFlagBits stage) :
        _device(device),
        _filename(filename), _stage(stage) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            Log::error("Unable to load shader source file: {}", filename);
        }

        _bytecode.resize(file.tellg());
        file.seekg(0);
        file.read(_bytecode.data(), _bytecode.size());
        file.close();

        SpvReflectShaderModule module;
        SpvReflectResult result = spvReflectCreateShaderModule(_bytecode.size(),
                                                               _bytecode.data(),
                                                               &module);
        DYN_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        parse_push_constants(module);
        parse_descriptor_layouts(module);
    }

    void
    ShaderModule::parse_descriptor_layouts(SpvReflectShaderModule &module) {
        unsigned count = 0;
        SpvReflectDescriptorSet *spv_sets;
        spvReflectEnumerateDescriptorSets(&module, &count, nullptr);
        spvReflectEnumerateDescriptorSets(&module, &count, &spv_sets);

        for (unsigned i = 0; i < count; i++) {
            SpvReflectDescriptorSet &spv_set = spv_sets[i];

            // Enumerate each binding
            for (unsigned j = 0; j < spv_set.binding_count; j++) {
                SpvReflectDescriptorBinding &spv_binding = *spv_set.bindings[j];

                DescriptorBinding binding;
                binding.set = spv_binding.set;
                binding.binding.binding = spv_binding.binding;
                binding.binding.descriptorType =
                    static_cast<vk::DescriptorType>(
                        spv_binding.descriptor_type);
                binding.binding.stageFlags = _stage;

                // Compute the descriptor count
                binding.binding.descriptorCount = 1;
                for (unsigned k = 0; k < spv_binding.array.dims_count; k++) {
                    unsigned dim = spv_binding.array.dims[k];
                    binding.binding.descriptorCount *= dim;
                }
                _descriptor_bindings.push_back(binding);
            }
        }

#ifdef DYN_DEBUG
        Log::info("`{}` descriptor bindings", _filename);
        for (const DescriptorBinding &binding : _descriptor_bindings) {
            Log::info("* (set = {}, binding = {}) {}[{}]",
                      binding.set,
                      binding.binding.binding,
                      vk::to_string(binding.binding.descriptorType),
                      binding.binding.descriptorCount);
        }
        Log::info("");
#endif
    }

    void ShaderModule::parse_push_constants(SpvReflectShaderModule &module) {
        unsigned count = 0;
        SpvReflectBlockVariable *blocks;
        spvReflectEnumeratePushConstantBlocks(&module, &count, nullptr);
        spvReflectEnumeratePushConstantBlocks(&module, &count, &blocks);

        for (unsigned i = 0; i < count; i++) {
            SpvReflectBlockVariable &block = blocks[i];
            vk::PushConstantRange range;
            range.stageFlags = _stage;
            range.offset = block.offset;
            range.size = block.size;
            _push_constant_ranges.push_back(range);

#ifdef DYN_DEBUG
            Log::info("`{}` push constant: {} ({} b, offset = {})",
                      _filename,
                      block.name,
                      block.size,
                      block.offset);
            for (unsigned j = 0; j < block.member_count; j++) {
                SpvReflectBlockVariable &member = block.members[j];
                Log::info("* {} ({} b)", member.name, member.size);
            }
            Log::info("");
#endif
        }
    }

    vk::UniqueShaderModule ShaderModule::get_handle() {
        // Create the shader module from the file
        vk::ShaderModuleCreateInfo shader_info;
        shader_info.codeSize = _bytecode.size();
        shader_info.pCode = reinterpret_cast<unsigned *>(_bytecode.data());

        return _device.get().get_handle().createShaderModuleUnique(shader_info);
    }

    vk::ShaderStageFlagBits ShaderModule::get_stage() const { return _stage; }

    const std::vector<char> &ShaderModule::get_bytecode() { return _bytecode; }

    const std::string &ShaderModule::get_filename() const { return _filename; }

    const std::vector<vk::PushConstantRange> &
    ShaderModule::get_push_constant_ranges() const {
        return _push_constant_ranges;
    }

    const std::vector<DescriptorBinding> &
    ShaderModule::get_descriptor_bindings() const {
        return _descriptor_bindings;
    }
} // namespace Dynamo::Graphics::Vulkan