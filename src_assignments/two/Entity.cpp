#include "Entity.h"

Entity::Entity(const size_t id, const std::string &tag) {

}

bool Entity::isActive() const {
    return false;
}

const std::string &Entity::tag() const {
    return "";
}

const size_t Entity::id() const {
    return 0;
}

void Entity::destroy() {

}
