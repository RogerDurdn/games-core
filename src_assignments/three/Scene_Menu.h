#pragma once

#include <iostream>
#include "Scene.h"
#include <SFML/Graphics.hpp>


class Scene_Menu : Scene {
    std::vector<std::string> menuStrings;
    sf::Text menuText;
    std::vector<std::string> levelPaths;
    int menuIndex;

    void init();

public:

    void update();

    // Systems

    void sRender();

    void sDoAction();
};