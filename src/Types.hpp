#pragma once

namespace Dynamo {
    /**
     * @brief Standard boolean
     *
     */
    using b8 = bool;

    /**
     * @brief 8-bit integer
     *
     */
    using i8 = char;

    /**
     * @brief 16-bit integer
     *
     */
    using i16 = short;

    /**
     * @brief 32-bit integer
     *
     */
    using i32 = int;

    /**
     * @brief 64-bit integer
     *
     */
    using i64 = long long;

    /**
     * @brief Unsigned 8-bit integer
     *
     */
    using u8 = unsigned char;

    /**
     * @brief Unsigned 16-bit integer
     *
     */
    using u16 = unsigned short;

    /**
     * @brief Unsigned 32-bit integer
     *
     */
    using u32 = unsigned int;

    /**
     * @brief Unsigned 64-bit integer
     *
     */
    using u64 = unsigned long long;

    /**
     * @brief 32-bit floating point number
     *
     */
    using f32 = float;

    /**
     * @brief 64-bit double
     *
     */
    using f64 = double;

    /**
     * @brief Asserts to guarantee that each type is of the correct size
     *
     */
    static_assert(sizeof(b8) == 1);
    static_assert(sizeof(i8) == 1);
    static_assert(sizeof(i16) == 2);
    static_assert(sizeof(i32) == 4);
    static_assert(sizeof(i64) == 8);
    static_assert(sizeof(u8) == 1);
    static_assert(sizeof(u16) == 2);
    static_assert(sizeof(u32) == 4);
    static_assert(sizeof(u64) == 8);
    static_assert(sizeof(f32) == 4);
    static_assert(sizeof(f64) == 8);
} // namespace Dynamo