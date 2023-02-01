#pragma once

#include <array>
#include <memory>

#include "../Math/Box2.hpp"
#include "../Math/Vec2.hpp"

namespace Dynamo {
    /**
     * @brief Direction vectors for each quadrant of the bounding box
     *
     */
    static const std::array<Vec2, 4> QUADRANT_DIRECTIONS = {
        Vec2(1, 1),   // TR
        Vec2(-1, 1),  // TL
        Vec2(-1, -1), // BL
        Vec2(1, -1),  // BR
    };

    /**
     * @brief A quadtree spatially subdivides nodes into quadrants when its
     * members reach maximum capacity.
     *
     * @tparam T        Item type.
     * @tparam Capacity Maximum number of items per node before subdividing.
     * @tparam MaxDepth Maximum depth of the tree.
     */
    template <typename T, unsigned Capacity, unsigned MaxDepth>
    class QuadTree : public Box2 {
        std::vector<T> _items;

        std::vector<std::unique_ptr<QuadTree>> _children;
        u32 _level;

        /**
         * @brief Split the node into quadrants and propagate items.
         *
         */
        void subdivide() {
            Vec2 halfdim = (max - min) * 0.5;
            for (u32 i = 0; i < 4 && _level != MaxDepth; i++) {
                _children.push_back(std::make_unique<QuadTree>(
                    center() + QUADRANT_DIRECTIONS[i],
                    halfdim.x,
                    halfdim.y,
                    _level + 1));
                for (T &item : _items) {
                    _children.back()->insert(item);
                }
            }

            // Items were moved down the tree
            if (!_children.empty()) {
                _items.clear();
            }
        }

        /**
         * @brief Test if the quadtree node collides with the item.
         *
         * @param item
         * @return true
         * @return false
         */
        b8 collides_with(const T &item) {
            if constexpr (std::is_convertible_v<T, Vec2>) {
                return contains(item);
            } else if constexpr (std::is_convertible_v<T, Box2>) {
                return intersects(item);
            }
            return false;
        }

      public:
        /**
         * @brief Construct a new QuadTree object
         *
         * @param min
         * @param max
         * @param level
         */
        QuadTree(Vec2 min, Vec2 max, u32 level = 0) :
            Box2(min, max), _level(level) {}

        /**
         * @brief Construct a new QuadTree object
         *
         * @param center
         * @param width
         * @param height
         * @param level
         */
        QuadTree(Vec2 center, f32 width, f32 height, u32 level = 0) :
            Box2(center, width, height), _level(level) {}

        /**
         * @brief Insert an item to the tree.
         *
         * @param item
         */
        void insert(const T &item) {
            if (_items.size() == Capacity) {
                subdivide();
            }
            if (_children.size() == 0 && collides_with(item)) {
                _items.push_back(item);
            } else {
                for (std::unique_ptr<QuadTree> &child : _children) {
                    child->insert(item);
                }
            }
        }

        /**
         * @brief Get the items that occupy the same node as the one queried.
         *
         * @param item
         * @return std::vector<T>
         */
        std::vector<T> get_neighbors(const T &item) {
            std::vector<T> neighbors;
            std::vector<std::reference_wrapper<QuadTree>> stack;
            stack.push_back(*this);

            while (!stack.empty()) {
                QuadTree &curr = stack.back();
                stack.pop_back();

                if (curr._children.empty()) {
                    for (const T &item : curr._items) {
                        neighbors.push_back(item);
                    }
                } else {
                    for (std::unique_ptr<QuadTree> &child : curr._children) {
                        if (child->collides_with(item)) {
                            stack.push_back(*child);
                        }
                    }
                }
            }
            return neighbors;
        }

        /**
         * @brief Clear the tree.
         *
         */
        void clear() {
            _children.clear();
            _items.clear();
        }
    };
} // namespace Dynamo