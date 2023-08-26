#pragma once

#include "Entity.h"
#include <iostream>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager {
    EntityVec m_entities;
    EntityMap m_entityMap;
    EntityVec m_toAdd;

    void init() {

    }

public:
    void update();

    std::shared_ptr<Entity> addEntity(const std::string &tag);

    const EntityVec &getEntities();

    const EntityVec &getEntities(const std::string &tag);
};
