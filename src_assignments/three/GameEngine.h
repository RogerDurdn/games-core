#pragma once

#include "Scene.h"
#include "Assets.h"


typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine {
    SceneMap scenes;
    sf::RenderWindow window;
    std::shared_ptr<Assets> assets;
    std::string currentScene;
    bool running;

    void init() {

    }

    std::shared_ptr<Scene> getCurrentScene();

public:
    void run();

    void update();

    void quit();

    void changeScene(const std::shared_ptr<Scene> scene);

    std::shared_ptr<Assets> getAssets() const;

    sf::Window &getWindow();

    void sUserInput();
};
