#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "../Math/Common.hpp"
#include "../Math/Complex.hpp"
#include "../Math/Delaunay.hpp"
#include "../Math/Quaternion.hpp"
#include "../Math/Triangle2.hpp"
#include "../Math/Vec2.hpp"
#include "../Math/Vec3.hpp"
#include "../Utils/ChannelData.hpp"

#include "./Filters/Convolver.hpp"

namespace Dynamo {
    /**
     * @brief Head-related transfer function for producing realistic 3D sounds
     * relative to a listener
     *
     */
    class HRTF {
        /**
         * @brief Maps azimuth-elevation angle pairs to coefficient sets
         *
         */
        std::unordered_map<Vec2, ChannelData<float>> _coeffs;

        /**
         * @brief Triangle mesh
         *
         */
        std::vector<Triangle2> _triangles;

        /**
         * @brief Length of the filter
         *
         */
        unsigned _filter_length;

      public:
        /**
         * @brief Load an HRTF dataset binary
         *
         * @param filename Path to the dataset
         */
        HRTF(std::string filename);

        /**
         * @brief Get the length of the filter coefficient buffer
         *
         * @return unsigned
         */
        unsigned get_filter_length();

        /**
         * @brief Get the Filter coefficients for a source relative to the
         * listener, applying interpolation as needed
         *
         * @param listener_position Position of the listener
         * @param listener_rotation Rotation of the listener
         * @param source_position   Position of the sound source
         * @param dst_buffer        Destination buffer
         */
        void get_coefficients(const Vec3 &listener_position,
                              const Quaternion &listener_rotation,
                              const Vec3 &source_position,
                              ChannelData<Complex> &dst_buffer);
    };
} // namespace Dynamo