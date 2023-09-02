#include "Entity.h"


bool Entity::isActive() const {
    return m_active;
}

void Entity::destroy() {
    this->m_active = false;
}

const std::string &Entity::tag() const {
    return m_tag;
}

const size_t Entity::id() const {
    return m_id;
}
