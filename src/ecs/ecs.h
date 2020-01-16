#ifndef DYNAMO_ECS_H_
#define DYNAMO_ECS_H_

#include <vector>
#include <map>
#include <algorithm>
#include <typeinfo>
#include <typeindex>
#include <cstdlib>
#include <iostream>

namespace Dynamo {
    // Implicit maximum of 2^32 - 1 possible unique entities
    typedef unsigned long Entity;

    struct BaseComponent {
        Entity owner = -1;

        virtual ~BaseComponent() = default;
    };

    template <class T>
    struct Component : BaseComponent {
        std::type_index name;

        Component() : name(typeid(T)) {};
    };

    // Keeps track of entities and components
    class EntityRegistry {
        Entity unique_id_;

        std::map<Entity, std::map<std::type_index, BaseComponent *>> entities_;
        std::map<std::type_index, std::vector<BaseComponent *>> components_;

    public:
        EntityRegistry();

        Entity create_entity();

        void destroy_entity(Entity id);

        template <typename T>
        void set_component(Entity entity, T prefab) {
            std::type_index component_type = typeid(T);
            if(entities_[entity].count(component_type)) {
                return;
            }

            T *component;
            memcpy(component, &prefab, sizeof(prefab));
            component->owner = entity;

            entities_[entity][component_type] = component;
            components_[component_type].push_back(component);
        };

        template <typename T>
        T *get_component(Entity entity) {
            std::type_index component_type = typeid(T);
            if(!entities_[entity].count(component_type)) {
                return nullptr;
            }

            BaseComponent *base = entities_[entity][component_type];
            return static_cast<T *>(base);
        }
    };

    class System {
        EntityRegistry *entities_;
        std::vector<std::type_index> registered_;    
        
    public:
        System(EntityRegistry *entities);

        template <typename T>
        void register_component() {
            std::type_index component_type = typeid(T);
            for(auto &type : registered_) {
                if(component_type == type) {
                    return;
                }
            }
            registered_.push_back(component_type);
        }

        virtual void update(unsigned dt) = 0;
    };
}

#endif