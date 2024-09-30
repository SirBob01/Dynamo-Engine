#pragma once

#include <functional>

namespace Dynamo {
    /**
     * @brief Thread-safe, reference-counted pointer like std::shared_ptr, but
     * with better cache coherency and a smaller memory footprint.
     *
     * This is intended to be used with all Dynamo::Object types.
     *
     * @tparam T
     */
    template <typename T>
    class Ref {
        T *_ptr;

        friend std::hash<Dynamo::Ref<T>>;

        /**
         * @brief Construct a new Ref from an existing pointer.
         *
         * @param ptr
         */
        Ref(T *ptr) : _ptr(ptr) {}

        /**
         * @brief Dereference or destroy.
         *
         */
        inline void deref_or_destroy() {
            if (_ptr->_ref_count == 1) {
                delete _ptr;
            } else {
                _ptr->deref();
            }
        }

      public:
        /**
         * @brief Construct an empty Ref.
         *
         */
        Ref() : _ptr(nullptr) {}

        /**
         * @brief Construct a new Ref from an existing reference.
         *
         * @param rhs
         */
        Ref(Ref<T> &rhs) : _ptr(rhs._ptr) { _ptr->ref(); }

        /**
         * @brief Construct a new Ref from an r-value reference.
         *
         * @param ref
         */
        Ref(Ref<T> &&rhs) : _ptr(rhs._ptr) { rhs._ptr = nullptr; }

        /**
         * @brief Destroy the Ref object if its reference count is 0.
         *
         */
        ~Ref() {
            if (_ptr != nullptr) {
                deref_or_destroy();
                _ptr = nullptr;
            }
        }

        /**
         * @brief Construct a new Ref object.
         *
         * @tparam Args
         * @param args
         */
        template <typename... Args>
        static Ref<T> create(Args... args) {
            return Ref<T>(new T(args...));
        }

        /**
         * @brief Check if the reference is empty.
         *
         * @return true
         * @return false
         */
        inline bool empty() const { return _ptr == nullptr; }

        /**
         * @brief Get the reference count.
         *
         * @return unsigned
         */
        inline unsigned ref_count() const {
            if (_ptr == nullptr) {
                return 0;
            } else {
                return _ptr->_ref_count;
            }
        }

        /**
         * @brief Dereference operator.
         *
         * @return T&
         */
        inline T &operator*() { return *_ptr; }

        /**
         * @brief Pointer accessor operator.
         *
         * @return T*
         */
        inline T *operator->() const { return _ptr; }

        /**
         * @brief Equality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const Ref<T> &rhs) const {
            return rhs._ptr == _ptr;
        }

        /**
         * @brief Inequality operator.
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator!=(const Ref<T> &rhs) const {
            return rhs._ptr != _ptr;
        }

        /**
         * @brief Copy assignment operator.
         *
         * @param rhs
         * @return Ref&
         */
        inline Ref &operator=(const Ref<T> &rhs) {
            if (rhs != *this) {
                if (_ptr != nullptr) {
                    deref_or_destroy();
                }
                _ptr = rhs._ptr;
                _ptr->ref();
            }
            return *this;
        }

        /**
         * @brief Move assignment operator.
         *
         * Decrements the current pointer value and invalidates the other one.
         *
         * @param other
         * @return Ref&
         */
        inline Ref &operator=(Ref<T> &&rhs) {
            if (rhs != *this) {
                if (_ptr != nullptr) {
                    deref_or_destroy();
                }
                _ptr = rhs._ptr;
                rhs._ptr = nullptr;
            }
            return *this;
        }
    };
} // namespace Dynamo

/**
 * @brief Hash function implementation for Quaternion.
 *
 * @tparam
 */
template <typename T>
struct std::hash<Dynamo::Ref<T>> {
    inline size_t operator()(const Dynamo::Ref<T> &ref) const {
        return std::hash(ref._ptr);
    }
};
