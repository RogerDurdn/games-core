#pragma once

#include "EntityManager.h"
#include "Action.h"
#include <memory>

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene {

protected:
    GameEngine *m_game = nullptr;
    EntityManager m_entityManager;
    ActionMap m_actionMap;
    bool m_paused = false;
    bool m_hasEnded = false;
    size_t m_currentFrame = 0;

    virtual void onEnd() = 0;

    Scene(GameEngine *gameEngine) : m_game(gameEngine) {
    }

    void setPaused(bool paused) {
        m_paused = true;
    }

public:
    Scene();


    virtual void update() = 0;

    virtual void sDoAction(const Action &action) = 0;

    virtual void sRender() = 0;


    void simulate(const size_t frames);

    void registerAction(Action action);

    void registerAction(int inputKey, const std::string &actionName) {
        this->m_actionMap[inputKey] = actionName;
    }

    int width() const {
        return 1280;
    }

    int height() const {
        return 768;
    }

    size_t currentFrame() const {
        return m_currentFrame;
    }

    bool hasEnded() const {
        return m_hasEnded;
    }

    const ActionMap &getActionMap() {
        return m_actionMap;
    }

    void drawLine(const Vec2 &p1, const Vec2 &p2);
};
