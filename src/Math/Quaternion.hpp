namespace Dynamo {
    /**
     * @brief Quaternion representation for rotations
     *
     */
    struct Quaternion {
        float x, y, z, w;

        /**
         * @brief Construct a new Quaternion object
         *
         * @param x
         * @param y
         * @param z
         * @param w
         */
        Quaternion(float x, float y, float z, float w) :
            x(x), y(y), z(z), w(w) {}
    };
} // namespace Dynamo