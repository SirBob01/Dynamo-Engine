namespace Dynamo {
    /**
     * @brief Customizable flags for initializing the engine
     *
     */
    enum class EngineFlag {
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

    inline unsigned operator&(EngineFlag lhs, EngineFlag rhs) {
        using T = unsigned;
        return static_cast<T>(lhs) & static_cast<T>(rhs);
    }

    inline EngineFlag operator|(EngineFlag lhs, EngineFlag rhs) {
        using T = unsigned;
        return static_cast<EngineFlag>(static_cast<T>(lhs) |
                                       static_cast<T>(rhs));
    }

    inline EngineFlag operator|=(EngineFlag &lhs, EngineFlag rhs) {
        lhs = lhs | rhs;
        return lhs;
    }
} // namespace Dynamo