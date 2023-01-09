#include "./Sampler.hpp"

namespace Dynamo::Graphics::Vulkan {
    Sampler::Sampler(Device &device, unsigned max_lod) : _device(device) {
        vk::SamplerCreateInfo sampler_info;
        sampler_info.magFilter = vk::Filter::eLinear;
        sampler_info.minFilter = vk::Filter::eLinear;
        sampler_info.mipmapMode = vk::SamplerMipmapMode::eLinear;

        sampler_info.addressModeU = vk::SamplerAddressMode::eRepeat;
        sampler_info.addressModeV = vk::SamplerAddressMode::eRepeat;
        sampler_info.addressModeW = vk::SamplerAddressMode::eRepeat;

        sampler_info.mipLodBias = 0.0f;

        sampler_info.anisotropyEnable = true;
        sampler_info.maxAnisotropy =
            _device.get().get_physical().get_limits().maxSamplerAnisotropy;

        sampler_info.compareEnable = false;
        sampler_info.compareOp = vk::CompareOp::eAlways;

        sampler_info.minLod = 0.0f;
        sampler_info.maxLod = max_lod;

        sampler_info.borderColor = vk::BorderColor::eIntOpaqueBlack;
        sampler_info.unnormalizedCoordinates = false;

        _handle = _device.get().get_handle().createSampler(sampler_info);
    }

    Sampler::~Sampler() { _device.get().get_handle().destroySampler(_handle); }

    const vk::Sampler &Sampler::get_handle() const { return _handle; }
} // namespace Dynamo::Graphics::Vulkan