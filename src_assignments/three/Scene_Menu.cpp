#include "Scene_Menu.h"
#include "GameEngine.h"

Scene_Menu::Scene_Menu(GameEngine *gameEngine)
        : Scene(gameEngine) {
    init();
}

void Scene_Menu::init() {
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::Enter, "PLAY");
    registerAction(sf::Keyboard::Up, "UP");
    registerAction(sf::Keyboard::Down, "DOWN");

    // TODO: Register all other gameplay Actions
    m_menuText = sf::Text();
    m_menuText.setFont(m_game->assets().getFont("Menu"));
}

void Scene_Menu::update() {
    sRender();
}

void Scene_Menu::sRender() {
    auto &window = m_game->window();
    window.clear(sf::Color(61, 116, 117));

    m_menuText.setStyle(sf::Text::Bold);
    m_menuText.setOutlineThickness(10);
    m_menuText.setCharacterSize(75);
    m_menuText.setString("MEGA MARIO");
    m_menuText.setPosition(25, 20);
    window.draw(m_menuText);

    m_menuText.setPosition(m_menuText.getPosition().x, m_menuText.getPosition().y + 75);
    std::string levels[3] = {"LEVEL 1", "LEVEL 2", "LEVEL 3"};
    for (int i = 0; i < std::size(levels); i++) {
        auto textLevel = levels[i];
        if (i == m_selectedMenuIndex) {
            std::cout << "index:" << i << "\n";
            textLevel = ">> " + textLevel + " <<";
        }
        m_menuText.setCharacterSize(60);
        m_menuText.setString(textLevel);
        m_menuText.setPosition(100, m_menuText.getPosition().y + m_menuText.getCharacterSize() + 55);
        window.draw(m_menuText);
    }
    m_menuText.setString("UP : W     DOWN : S   PLAY : D     BACK : ESC");
    m_menuText.setCharacterSize(25);
    m_menuText.setOutlineThickness(5);
    m_menuText.setPosition(30, m_game->window().getSize().y - 65);
    window.draw(m_menuText);

    window.display();
}

void Scene_Menu::sDoAction(const Action &action) {
    if (action.type() == "START") {
        if (action.name() == "QUIT") { exit(0); }
        else if (action.name() == "UP") {
            m_selectedMenuIndex = (m_selectedMenuIndex == 0) ? 0 : m_selectedMenuIndex - 1;
            std::cout << "up\n";
        } else if (action.name() == "DOWN") {
            std::cout << "down\n";
            m_selectedMenuIndex = (m_selectedMenuIndex == 2) ? 2 : m_selectedMenuIndex + 1;
        }
    }
}

void Scene_Menu::onEnd() {
    // TODO: when the scene ends, change back to the Menu scene use m_game -> changeScene(params)
}
