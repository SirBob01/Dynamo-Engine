#pragma once

#include <array>

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
     * @brief QuadTree spatially subdivides nodes into quadrants when its
     * members reach maximum capacity
     *
     * @tparam T        Item type
     * @tparam Capacity Maximum number of items per node
     * @tparam MaxDepth Maximum depth of the tree
     */
    template <typename T, unsigned Capacity, unsigned MaxDepth>
    class QuadTree : public Box2 {
        /**
         * @brief Node holds indexes to its children and its first item node
         *
         */
        struct Node {
            unsigned child_index = -1;
            unsigned member_index = -1;
            unsigned count = 0;

            /**
             * @brief Test if the node is a leaf
             *
             * @return true
             * @return false
             */
            inline bool leaf() { return child_index == -1; }
        };
        std::vector<Node> _nodes;

        /**
         * @brief Members hold the the item and the index to the next item
         *
         */
        struct Member {
            unsigned next_index = -1;
            T item;
        };
        std::vector<Member> _members;

        /**
         * @brief Structure used for querying the tree, containg a reference to
         * the node and its bounding volume
         *
         * This is computed on the fly rather than stored
         *
         */
        struct Query {
            Box2 bounds;
            unsigned node_index;
            unsigned depth;
        };

        /**
         * @brief Calculate the quadrant bounds of a parent
         *
         * @param parent
         * @param index
         * @return Box2
         */
        inline Box2 calculate_quadrant(const Box2 &parent, unsigned index) {
            const Vec2 &direction = QUADRANT_DIRECTIONS[index];
            const Vec2 half_size = (parent.max - parent.min) * 0.5;
            Vec2 n_center = parent.center();

            // Calculate new center
            n_center.x += direction.x * half_size.x * 0.5;
            n_center.y += direction.y * half_size.y * 0.5;

            return Box2(n_center, half_size.x, half_size.y);
        }

        /**
         * @brief Test if an item collides with a bounding volume
         *
         * @param bounds
         * @param item
         * @return true
         * @return false
         */
        inline bool collides(const Box2 &bounds, const T &item) {
            if constexpr (std::is_convertible_v<T, Vec2>) {
                return bounds.contains(item);
            } else if constexpr (std::is_convertible_v<T, Box2>) {
                return bounds.intersects(item);
            }
            return false;
        }

        /**
         * @brief Add an item to an internal node
         *
         * @param root
         * @param item
         */
        void insert_inode(const Query &root, const T &item) {
            std::vector<Query> stack = {root};
            while (!stack.empty()) {
                Query query = stack.back();
                stack.pop_back();

                Box2 &bounds = query.bounds;
                if (!collides(bounds, item)) {
                    // Item doesn't belong in this node
                    continue;
                } else if (_nodes[query.node_index].leaf()) {
                    // No children, this is a leaf node
                    insert_leaf(query, item);
                    continue;
                }

                // Generate queries for the children, mark them for processing
                unsigned offset = _nodes[query.node_index].child_index;
                for (int i = 0; i < 4; i++) {
                    Query child = {
                        calculate_quadrant(bounds, i),
                        offset + i,
                        query.depth + 1,
                    };
                    stack.push_back(child);
                }
            }
        }

        /**
         * @brief Add an item to a leaf node
         *
         * @param query
         * @param item
         */
        void insert_leaf(const Query &query, const T &item) {
            if (_nodes[query.node_index].count == Capacity &&
                query.depth < MaxDepth) {
                subdivide(query);
                insert_inode(query, item);
            } else {
                // Update members
                Member member;
                member.item = item;
                member.next_index = _nodes[query.node_index].member_index;

                // Update node
                _nodes[query.node_index].member_index = _members.size();
                _nodes[query.node_index].count++;

                _members.push_back(member);

                std::cout << member.next_index << " "
                          << _nodes[query.node_index].member_index << " "
                          << _nodes[query.node_index].count << "\n";
            }
        }

        void subdivide(const Query &query) {
            // Create new nodes
            for (int i = 0; i < 4; i++) {
                _nodes.emplace_back();
            }
            _nodes[query.node_index].child_index = _nodes.size() - 4;

            // Iterate over each item and propagate it to the children
            unsigned member_index = _nodes[query.node_index].member_index;
            while (member_index != -1) {
                Member &member = _members[member_index];
                insert_inode(query, member.item);
                member_index = member.next_index;
            }
            _nodes[query.node_index].member_index = -1;
            _nodes[query.node_index].count = 0;
        }

      public:
        /**
         * @brief Construct a new QuadTree object
         *
         * @param min
         * @param max
         */
        QuadTree(Vec2 min, Vec2 max) : Box2(min, max) { _nodes.emplace_back(); }

        /**
         * @brief Construct a new QuadTree object given its center and
         * dimensions
         *
         * @param center
         * @param width
         * @param height
         */
        QuadTree(Vec2 center, float width, float height) :
            Box2(center, width, height) {
            _nodes.emplace_back();
        }

        /**
         * @brief Get the maximum number of members a node can hold before
         * subdividing
         *
         * @return constexpr unsigned
         */
        constexpr unsigned capacity() const { return Capacity; }

        /**
         * @brief Get the maximum depth of the tree
         *
         * @return constexpr unsigned
         */
        constexpr unsigned max_depth() const { return MaxDepth; }

        /**
         * @brief Insert an item
         *
         * @param item
         */
        void insert(const T &item) {
            if (!collides(*this, item)) {
                return;
            }
            Query root = {*this, 0, 0};
            if (_nodes[root.node_index].leaf()) {
                insert_leaf(root, item);
            } else {
                insert_inode(root, item);
            }
        }

        /**
         * @brief Remove an item
         *
         * @param item
         */
        void remove(const T &item) {}

        /**
         * @brief Test if an item exists in the tree
         *
         * @param item
         * @return true
         * @return false
         */
        bool contains(const T &item) const { return true; }

        /**
         * @brief Find the neighboring members of the item
         *
         * @param item
         * @return std::vector<T>
         */
        std::vector<T> search(const T &item) const {
            std::vector<T> result;
            return result;
        }

        /**
         * @brief Deferred clean-up will remove all empty cells, making the
         * element and node arrays contiguous
         *
         */
        void cleanup() {}

        /**
         * @brief Empty the quadtree, leaving only the root node
         *
         */
        void clear() {
            _members.clear();
            _nodes.resize(1);

            _nodes[0].child_index = -1;
            _nodes[0].member_index = -1;
            _nodes[0].count = 0;
        }
    };
} // namespace Dynamo