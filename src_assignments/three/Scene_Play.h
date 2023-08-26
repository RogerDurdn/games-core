#pragma once

#include <iostream>
#include "Scene.h"
#include "Entity.h"
#include "Action.h"

struct PlayerConfig {

};

class Scene_Play : Scene {

    std::string levelPath;
    std::shared_ptr<Entity> player;
    std::shared_ptr<PlayerConfig> playerConfig;

    void init();

public:

    // Systems
    void sAnimation();

    void sMovement();

    void sEnemySpawner();

    void sCollision();

    void sRender();

    void sDoAction(Action);

    void sDebug();

};


