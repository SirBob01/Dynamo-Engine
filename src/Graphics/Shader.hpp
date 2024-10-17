#pragma once

#include <string>

#include <Utils/IdTracker.hpp>

namespace Dynamo::Graphics {
    /**
     * @brief Shader handle.
     *
     */
    using Shader = Id;

    /**
     * @brief Shader pipeline stage.
     *
     */
    enum class ShaderStage {
        Vertex,
        Fragment,
        Compute,
        TesselationControl,
        TesselationEvaluation,
    };

    /**
     * @brief Shader descriptor.
     *
     */
    struct ShaderDescriptor {
        /**
         * @brief Name of the shader.
         *
         */
        std::string name;

        /**
         * @brief Shader source code in the native language of the renderer backend.
         *
         * Source must have a `void main()` entry function.
         *
         */
        std::string code;

        /**
         * @brief Shader stage.
         *
         */
        ShaderStage stage;
    };
} // namespace Dynamo::Graphics