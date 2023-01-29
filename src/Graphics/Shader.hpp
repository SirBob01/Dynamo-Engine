#pragma once

#include <string>
#include <vector>

namespace Dynamo::Graphics {
    /**
     * @brief Enumerates the shader stages
     *
     */
    enum class ShaderStage : u32 {
        /**
         * @brief Vertex processing shader
         *
         */
        Vertex,

        /**
         * @brief Primitive processing shader
         *
         */
        Geometry,

        /**
         * @brief Pixel rasterization shader
         *
         */
        Fragment,

        /**
         * @brief General purpose computation shader
         *
         */
        Compute,
    };

    /**
     * @brief Rendering shader program
     *
     */
    class Shader {
      public:
        /**
         * @brief Destroy the Shader object
         *
         */
        virtual ~Shader() = default;

        /**
         * @brief Get the source filename
         *
         * @return const std::string
         */
        virtual const std::string &get_filename() const = 0;

        /**
         * @brief Get the bytecode
         *
         * @return const std::vector<u8>&
         */
        virtual const std::vector<char> &get_bytecode() const = 0;

        /**
         * @brief Get the shader stage
         *
         * @return ShaderStage
         */
        virtual ShaderStage get_stage() const = 0;
    };

    /**
     * @brief Array of shader modules
     *
     */
    using ShaderList = std::vector<std::reference_wrapper<Shader>>;
} // namespace Dynamo::Graphics