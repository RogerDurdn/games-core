#include "GameEngine.h"
#include "Assets.h"
#include "Scene_Play.h"
#include "Scene_Menu.h"


GameEngine::GameEngine(const std::string &path) {
    init(path);
}

void GameEngine::init(const std::string &path) {
    m_assets.loadFromFile(path);
    m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
    m_window.setFramerateLimit(60);

    changeScene("MENU", std::make_shared<Scene_Menu>(this), false);
}

std::shared_ptr<Scene> GameEngine::currentScene() {
    return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning() {
    return m_running & m_window.isOpen();
}

sf::RenderWindow &GameEngine::window() {
    return m_window;
}

void GameEngine::run() {
    while (isRunning()) {
        update();
    }
}


void GameEngine::sUserInput() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            quit();
        }
//        if (event.type == sf::Event::KeyPressed) {
//            std::cout << "screenshot saved to" << "test.png" << std::endl;
//            sf::Texture texture;
//            texture.create(m_window.getSize().x, m_window.getSize().y);
//            texture.update(m_window);
//            if (texture.copyToImage().saveToFile("test.png")) {
//                std::cout << "screenshot saved to" << "test.png" << std::endl;
//            }
//        }
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) {
                continue;
            }
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";
            currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }
        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
            const std::string actionType = (event.type == sf::Event::MouseButtonPressed) ? "START" : "END";
            auto pos = sf::Mouse::getPosition(m_window);
            switch (event.mouseButton.button) {
                case sf::Mouse::Left:
                    currentScene()->sDoAction(Action("mouseLeft", actionType, Vec2(pos.x, pos.y)));
                    break;
            }
        }

        if (event.type == sf::Event::MouseMoved) {
            currentScene()->sDoAction(
                    Action("mouseMove", "START", Vec2(event.mouseMove.x, event.mouseMove.y)));
        }
    }
}

void GameEngine::changeScene(const std::string &name, const std::shared_ptr<Scene> scene, bool endCurrentScene) {
    m_sceneMap[name] = scene;
    m_currentScene = name;
    std::cout << "scene:" << name;
}

void GameEngine::update() {
    sUserInput();
    currentScene()->update();
    m_window.display();
}

void GameEngine::quit() {
    exit(0);
}

const Assets &GameEngine::assets() const {
    return m_assets;
}
