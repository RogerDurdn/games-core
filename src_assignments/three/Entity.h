#pragma once

#include "Component.h"

class Entity {
    friend class EntityManager;

    std::tuple<
            std::shared_ptr<CTransform>,
            std::shared_ptr<CShape>,
            std::shared_ptr<CCollision>,
            std::shared_ptr<CInput>,
            std::shared_ptr<CScore>,
            std::shared_ptr<CLifespan>> components;

    std::string m_tag = "default";
    bool m_active = true;
    size_t m_id = 0;

    //constructor and destructor
    Entity(const size_t id, const std::string &tag);

public:

    void addComponent(const Component &component);

    bool hasComponent(const Component component) const;

    Component &getComponent(const Component component) const;

    void removeComponent(const Component component) const;

    // component pointers
    bool isActive() const;

    const std::string &tag() const;

    const size_t id() const;

    void destroy();
};
