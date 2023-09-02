#include "Entity.h"
#include "Component.h"


Entity::Entity(const size_t id, const std::string &tag) : m_id(id), m_tag(tag) {
    m_components = ComponentTuple(
            CTransform(),
            CLifespan(),
            CInput(),
            CBoundingBox(),
            CAnimation(),
            CGravity(),
            CState());
}

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
