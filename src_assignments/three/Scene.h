#pragma once

#include "GameEngine.h"
#include "EntityManager.h"
#include "Action.h"
#include <iostream>

class Scene {
    std::shared_ptr<GameEngine> game;
    std::shared_ptr<EntityManager> entities;
    int currentFrame;
    std::map<int, std::string> actionMap;
    bool paused;
    bool hasEnded;
public:
    virtual void update() = 0;

    virtual void sDoAction() = 0;

    virtual void sRender() = 0;

    void simulate(int id);

    void doAction(Action action);

    void registerAction(Action action);
};
