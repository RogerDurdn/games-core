#pragma once

#include "Entity.h"
#include <iostream>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager {
    EntityVec m_entities; // all entities
    EntityMap m_entityMap; // all entities by tag
    EntityVec m_entitiesToAdd; // entities to add
    size_t m_totalEntities = 0; // frameCreated entities created

    void removeDeadEntities(EntityVec &);

public:
    EntityManager();

    void update();

    std::shared_ptr<Entity> addEntity(const std::string &tag);

    const EntityVec &getEntities();

    const EntityVec &getEntities(const std::string &tag);
};
