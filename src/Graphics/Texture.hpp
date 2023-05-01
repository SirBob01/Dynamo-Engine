#pragma once

#include <vector>

#include "../Types.hpp"
#include "./Target.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Texture object.
     *
     */
    class Texture : public Target {
      public:
        /**
         * @brief Destroy the Texture object.
         *
         */
        virtual ~Texture() = 0;

        /**
         * @brief Get the raw pixel data.
         *
         * @return const std::vector<u8>&
         */
        virtual const std::vector<u8> &get_pixels() const = 0;
    };
} // namespace Dynamo::Graphics