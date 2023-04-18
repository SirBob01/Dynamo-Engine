#pragma once

#include <functional>
#include <string>
#include <vector>

#include <tiny_obj_loader.h>

#include "../../Graphics/Geometry.hpp"
#include "../../Log/Log.hpp"
#include "../../Types.hpp"

namespace Dynamo {
    /**
     * @brief Geometry loader.
     *
     */
    class GeometryLoader {
      public:
        /**
         * @brief Load geometry from a file.
         *
         * @param filepath
         * @return Graphics::Geometry
         */
        static Graphics::Geometry load(const std::string filepath);
    };
} // namespace Dynamo