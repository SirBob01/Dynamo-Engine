#pragma once

namespace Dynamo {
    /**
     * @brief Customizable flags for initializing the engine
     *
     */
    enum class ApplicationFlag : unsigned {
        /**
         * @brief None
         *
         */
        None = 0,

        /**
         * @brief Enable fullscreen mode
         *
         */
        FullScreen = 1 << 0,

        /**
         * @brief Enable vsync
         *
         */
        VSync = 1 << 1
    };

    /**
     * @brief AND operator
     *
     * @param lhs
     * @param rhs
     * @return unsigned
     */
    inline unsigned operator&(ApplicationFlag lhs, ApplicationFlag rhs) {
        using T = unsigned;
        return static_cast<T>(lhs) & static_cast<T>(rhs);
    }

    /**
     * @brief OR operator
     *
     * @param lhs
     * @param rhs
     * @return ApplicationFlag
     */
    inline ApplicationFlag operator|(ApplicationFlag lhs, ApplicationFlag rhs) {
        using T = unsigned;
        return static_cast<ApplicationFlag>(static_cast<T>(lhs) |
                                       static_cast<T>(rhs));
    }

    /**
     * @brief OR operator in-place
     *
     * @param lhs
     * @param rhs
     * @return ApplicationFlag
     */
    inline ApplicationFlag operator|=(ApplicationFlag &lhs, ApplicationFlag rhs) {
        lhs = lhs | rhs;
        return lhs;
    }
} // namespace Dynamo