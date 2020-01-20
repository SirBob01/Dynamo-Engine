#include "entity.h"

namespace Dynamo {
    EntityTracker::EntityTracker() {
        index_counter_ = 0;
    }

    uint32_t EntityTracker::get_index(Entity entity) {
        return entity >> 32;
    }

    uint32_t EntityTracker::get_version(Entity entity) {
        return entity & 0xFFFFFFFF;
    }

    bool EntityTracker::is_active(Entity entity) {
        uint32_t index, version;
        index = EntityTracker::get_index(entity);
        version = EntityTracker::get_version(entity);

        if(index + 1 > index_counter_) {
            return false;
        }
        else if(versions_[index] > version) {
            return false;
        }
        return true;
    }

    Entity EntityTracker::generate_id() {
        Entity entity = 0;
        uint32_t index, version;

        if(!free_pool_.empty()) {
            index = free_pool_.front();
            free_pool_.pop();
        }
        else {
            index = index_counter_++;
            versions_.push_back(0);
        }
        version = versions_[index];
        entity |= index;
        return (entity << 32) | version;
    }

    void EntityTracker::invalidate_id(Entity entity) {
        if(!is_active(entity)) {
            return;
        }
        uint32_t index = EntityTracker::get_index(entity);
        versions_[index]++;
        free_pool_.push(index);
    }

    void EntityTracker::clear() {
        index_counter_ = 0;
        versions_.clear();
        free_pool_ = {};
    }
}