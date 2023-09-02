#pragma once

#include "Scene.h"
#include "EntityManager.h"
#include <map>
#include <memory>
#include <deque>


class Scene_Menu : public Scene {

protected:
    std::string m_title;
    std::vector<std::string> m_menuStrings;
    std::vector<std::string> m_levelPaths;
    sf::Text m_menuText;
    size_t m_selectedMenuIndex = 0;

    void init();

    void onEnd();

    Scene_Menu();


public:

    Scene_Menu(GameEngine *gameEngine);

    void setPaused(bool paused);

    void update();

    void sDoAction(const Action &action);

    void sRender();

};