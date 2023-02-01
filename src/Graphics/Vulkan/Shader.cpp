#include "./Shader.hpp"

namespace Dynamo::Graphics::Vulkan {
    Shader::Shader(Device &device, const std::string filename) :
        _device(device), _filename(filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            Log::error("Unable to load shader source file: {}", filename);
        }

        _bytecode.resize(file.tellg());
        file.seekg(0);
        file.read(reinterpret_cast<i8 *>(_bytecode.data()), _bytecode.size());
        file.close();

        SpvReflectShaderModule module;
        SpvReflectResult result = spvReflectCreateShaderModule(_bytecode.size(),
                                                               _bytecode.data(),
                                                               &module);
        DYN_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        // Determine the shader stage
        switch (module.shader_stage) {
        case SpvReflectShaderStageFlagBits::SPV_REFLECT_SHADER_STAGE_VERTEX_BIT:
            _stage = ShaderStage::Vertex;
            break;
        case SpvReflectShaderStageFlagBits::
            SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT:
            _stage = ShaderStage::Geometry;
            break;
        case SpvReflectShaderStageFlagBits::
            SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT:
            _stage = ShaderStage::Fragment;
            break;
        case SpvReflectShaderStageFlagBits::
            SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT:
            _stage = ShaderStage::Compute;
            break;
        default:
            Log::error("Unsupported shader stage used: `{}`", filename);
            break;
        }

        parse_push_constants(module);
        parse_descriptor_layouts(module);
    }

    void Shader::parse_descriptor_layouts(SpvReflectShaderModule &module) {
        u32 count = 0;
        SpvReflectDescriptorSet *spv_sets;
        spvReflectEnumerateDescriptorSets(&module, &count, nullptr);
        spvReflectEnumerateDescriptorSets(&module, &count, &spv_sets);

        for (u32 i = 0; i < count; i++) {
            SpvReflectDescriptorSet &spv_set = spv_sets[i];

            // Enumerate each binding
            for (u32 j = 0; j < spv_set.binding_count; j++) {
                SpvReflectDescriptorBinding &spv_binding = *spv_set.bindings[j];
                vk::DescriptorType type = static_cast<vk::DescriptorType>(
                    spv_binding.descriptor_type);

                DescriptorBinding binding;
                binding.name = spv_binding.name;
                binding.set = spv_binding.set;
                binding.size = spv_binding.block.size;

                binding.binding.binding = spv_binding.binding;
                binding.binding.descriptorType = type;
                binding.binding.stageFlags = convert_shader_stage(_stage);

                // Compute the descriptor count
                binding.binding.descriptorCount = 1;
                for (u32 k = 0; k < spv_binding.array.dims_count; k++) {
                    u32 dim = spv_binding.array.dims[k];
                    binding.binding.descriptorCount *= dim;
                }

                // Determine the binding type
                switch (type) {
                case vk::DescriptorType::eUniformBuffer:
                case vk::DescriptorType::eStorageBuffer:
                case vk::DescriptorType::eUniformBufferDynamic:
                case vk::DescriptorType::eStorageBufferDynamic:
                case vk::DescriptorType::eUniformTexelBuffer:
                case vk::DescriptorType::eStorageTexelBuffer:
                    binding.type = BindingType::Buffer;
                    break;
                default:
                    binding.type = BindingType::Texture;
                    break;
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

    void Shader::parse_push_constants(SpvReflectShaderModule &module) {
        u32 count = 0;
        SpvReflectBlockVariable *blocks;
        spvReflectEnumeratePushConstantBlocks(&module, &count, nullptr);
        spvReflectEnumeratePushConstantBlocks(&module, &count, &blocks);

        for (u32 i = 0; i < count; i++) {
            SpvReflectBlockVariable &block = blocks[i];
            vk::PushConstantRange range;
            range.stageFlags = convert_shader_stage(_stage);
            range.offset = block.offset;
            range.size = block.size;
            _push_constant_ranges.push_back(range);

#ifdef DYN_DEBUG
            Log::info("`{}` push constant: {} ({} b, offset = {})",
                      _filename,
                      block.name,
                      block.size,
                      block.offset);
            for (u32 j = 0; j < block.member_count; j++) {
                SpvReflectBlockVariable &member = block.members[j];
                Log::info("* {} ({} b)", member.name, member.size);
            }
            Log::info("");
#endif
        }
    }

    vk::UniqueShaderModule Shader::get_handle() {
        // Create the shader module from the file
        vk::ShaderModuleCreateInfo shader_info;
        shader_info.codeSize = _bytecode.size();
        shader_info.pCode = reinterpret_cast<u32 *>(_bytecode.data());

        return _device.get().get_handle().createShaderModuleUnique(shader_info);
    }

    const std::string &Shader::get_filename() const { return _filename; }

    const std::vector<u8> &Shader::get_bytecode() const { return _bytecode; }

    ShaderStage Shader::get_stage() const { return _stage; }

    const std::vector<vk::PushConstantRange> &
    Shader::get_push_constant_ranges() const {
        return _push_constant_ranges;
    }

    const std::vector<DescriptorBinding> &
    Shader::get_descriptor_bindings() const {
        return _descriptor_bindings;
    }
} // namespace Dynamo::Graphics::Vulkan