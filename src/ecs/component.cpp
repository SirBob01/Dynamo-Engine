#include "component.h"

namespace Dynamo {
    ComponentInstance::~ComponentInstance() {
        delete[] data;
    }

    template <typename Component>
    Component *ComponentRegistry::get_component(Entity entity) {
        std::type_index type = typeid(Component);
        auto &component_bag = get_collection(entity);

        if(component_bag.find(type) != component_bag.end()) {
            return static_cast<Component *>(component_bag[type]->data);
        }
        return nullptr;
    }

    std::unordered_map<
        std::type_index, ComponentInstance *
    > &ComponentRegistry::get_collection(Entity entity) {
        if(entities_.find(entity) == entities_.end()) {
            throw InvalidKey(std::to_string(entity), "entities_");
        }
        return entities_[entity];
    }

    template <typename Component>
    void ComponentRegistry::set_component(Entity entity, Component prefab) {
        ComponentInstance instance;

        // Instantiate the component
        instance.owner = entity;
        instance.size = sizeof(Component);
        instance.data = new char[instance.size];
        memcpy(instance.data, &prefab, instance.size);

        // Assign it to the appropriate entity and component list
        std::type_index type = typeid(Component);
        components_[type].push_back(instance);
        entities_[entity][type] = &components_[type].back();
    }

    template<typename Component>
    void ComponentRegistry::remove_component(Entity entity) {
        std::type_index type = typeid(Component);
        auto &component_bag = get_collection(entity);
        auto &component_list = components_[type];

        if(component_bag.find(type) == component_bag.end()) {
            return;
        }
        component_list.erase(
            std::remove_if(
                component_list.begin(), 
                component_list.end(), 
                [entity](ComponentInstance &instance) {
                    return instance.owner == entity;
                }), 
            component_list.end()
        );
        component_bag.erase(type);
    }

    void ComponentRegistry::destroy_entity(Entity entity) {
        auto &component_bag = get_collection(entity);
        for(auto &item : component_bag) {
            auto &component_list = components_[item.first];
            component_list.erase(
                std::remove_if(
                    component_list.begin(), 
                    component_list.end(), 
                    [entity](ComponentInstance &instance) {
                        return instance.owner == entity;
                    }),
                component_list.end()
            );
        }
        entities_.erase(entity);
    }
}