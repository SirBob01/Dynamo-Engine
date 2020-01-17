#include "component.h"

namespace Dynamo {
    template <class Component>
    void ComponentRegistry::set_component(Entity entity, Component prefab) {
        ComponentInstance instance;

        // Instantiate the component
        instance.owner = entity;
        instance.size = sizeof(Component);
        instance.data = std::malloc(instance.size);
        memcpy(instance.data, &prefab, instance.size);

        // Assign it to the appropriate entity and component list
        std::type_index type = typeid(Component);
        components_[type].push_back(instance);
        entities_[entity][type] = &components_[type].back();
    }

    template<class Component>
    void ComponentRegistry::remove_component(Entity entity) {
        std::type_index type = typeid(Component);
        auto &component_bag = entities_[entity];
        if(component_bag.find(type) == component_bag.end()) {
            return;
        }
        components_.erase(
            std::remove_if(
                components_.begin(), 
                components_.end(), 
                [entity](ComponentInstance &instance) {
                    instance.owner == entity;
                }), 
            components_.end()
        );
        component_bag.erase(type);
    }

    template <class Component>
    Component *ComponentRegistry::get_component(Entity entity) {
        std::type_index type = typeid(Component);
        auto &component_bag = entities_[entity];

        if(component_bag.find(type) != component_bag.end()) {
            return static_cast<Component *>(component_bag[type]->data);
        }
        return nullptr;
    }
}