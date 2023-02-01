#pragma once

#include <string>

#include "../Math/Color.hpp"
#include "../Math/Matrix.hpp"
#include "../Math/Vec2.hpp"
#include "../Math/Vec3.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief A material describes the visual properties of a model.
     *
     */
    class Material {
      public:
        /**
         * @brief Destroy the Material object.
         *
         */
        virtual ~Material() = default;

        /**
         * @brief Write host local data to a uniform.
         *
         * @param field Name of the uniform variable within the shader.
         * @param src   Source data.
         * @param size  Size of the data.
         */
        virtual void set(const std::string field, u8 *src, u32 size) = 0;

        /**
         * @brief Set a float uniform.
         *
         * @param field Name of the uniform variable within the shader.
         * @param data  Source data.
         */
        inline void set_float(const std::string field, f32 data) {
            set(field, reinterpret_cast<u8 *>(&data), sizeof(data));
        }

        /**
         * @brief Set a double uniform.
         *
         * @param field Name of the uniform variable within the shader.
         * @param data  Source data.
         */
        inline void set_double(const std::string field, f64 data) {
            set(field, reinterpret_cast<u8 *>(&data), sizeof(data));
        }

        /**
         * @brief Set an int uniform.
         *
         * @param field Name of the uniform variable within the shader.
         * @param data  Source data.
         */
        inline void set_int(const std::string field, i32 data) {
            set(field, reinterpret_cast<u8 *>(&data), sizeof(data));
        }

        /**
         * @brief Set a Vec2 uniform.
         *
         * @param field Name of the uniform variable within the shader.
         * @param data  Source data.
         */
        inline void set_vec2(const std::string field, Vec2 &data) {
            set(field, reinterpret_cast<u8 *>(&data), sizeof(data));
        }

        /**
         * @brief Set a Vec3 uniform.
         *
         * @param field Name of the uniform variable within the shader.
         * @param data  Source data.
         */
        inline void set_vec3(const std::string field, Vec3 &data) {
            set(field, reinterpret_cast<u8 *>(&data), sizeof(data));
        }

        /**
         * @brief Set a matrix uniform of size N.
         *
         * @tparam N Dimensions of the matrix.
         * @param field Name of the uniform variable within the shader.
         * @param data  Source data.
         */
        template <u32 N>
        inline void set_matrix(const std::string field, Matrix<N> &data) {
            set(field,
                reinterpret_cast<u8 *>(data.values.data()),
                sizeof(data));
        }

        /**
         * @brief Set a color uniform.
         *
         * @param field Name of the uniform variable within the shader.
         * @param data  Source data.
         */
        inline void set_color(const std::string field, Color &data) {
            set(field, reinterpret_cast<u8 *>(&data), sizeof(data));
        }
    };
} // namespace Dynamo::Graphics
