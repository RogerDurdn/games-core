#pragma once

#include "Components.h"

class EntityManager;

typedef std::tuple <
CTransform,
CLifespan,
CInput,
CBoundingBox,
CAnimation,
CGravity,
CState,
CDraggable
> ComponentTuple;

class Entity {

    friend class EntityManager;

    bool m_active = true;
    std::string m_tag = "default";
    size_t m_id = 0;
    ComponentTuple m_components;


    //constructor is private, so we can never create entities outside the entityManager
    Entity(const size_t id, const std::string &tag);


public:

    void destroy();

    bool isActive() const;

    const std::string &tag() const;

    const size_t id() const;


    template<typename T>
    bool hasComponent() {
        return getComponent<T>().has;
    }

    template<typename T, typename... TArgs>
    T &addComponent(TArgs &&... mArgs) {
        auto &component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T, typename... TArgs>
    Entity *with(TArgs &&... mArgs) {
        auto &component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return this;
    }

    template<typename T>
    T &getComponent() {
        return std::get<T>(m_components);
    }

    template<typename T>
    void removeComponent() {
        getComponent<T>() = T();
    }

};
