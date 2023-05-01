#pragma once

#include <functional>
#include <string>

#include "../Types.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Enumerates the shader stages.
     *
     */
    enum class ShaderStage {
        /**
         * @brief Vertex processing shader.
         *
         */
        Vertex,

        /**
         * @brief Primitive processing shader.
         *
         */
        Geometry,

        /**
         * @brief Pixel rasterization shader.
         *
         */
        Fragment,

        /**
         * @brief General purpose computation shader.
         *
         */
        Compute,
    };

    /**
     * @brief Shader program module.
     *
     */
    class Shader {
      public:
        /**
         * @brief Destroy the Shader object.
         *
         */
        virtual ~Shader() = 0;

        /**
         * @brief Get the pipeline stage of this shader.
         *
         * @return ShaderStage
         */
        virtual ShaderStage get_stage() const = 0;

        /**
         * @brief Get the compiled shader byecode.
         *
         * @return const std::vector<u8>&
         */
        virtual const std::vector<u8> &get_bytecode() const = 0;
    };

    /**
     * @brief List of shader modules.
     *
     */
    using ShaderList = std::vector<std::reference_wrapper<Shader>>;
} // namespace Dynamo::Graphics