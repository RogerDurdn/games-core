#include "Scene_Menu.h"
#include "Scene_Zelda.h"
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

    m_menuText = sf::Text();
    m_menuText.setFont(m_game->assets().getFont("Menu"));
}

void Scene_Menu::update() {
    sRender();
}

void Scene_Menu::sRender() {
    m_game->window().clear(sf::Color(61, 116, 117));
    auto &menuText = m_menuText;
    auto &window = m_game->window();
    auto drawText = [&menuText, &window](const std::string &text, int size, float thick, float posX,
                                         float posY) -> void {
        menuText.setOutlineThickness(thick);
        menuText.setCharacterSize(size);
        menuText.setPosition(posX, posY);
        menuText.setString(text);
        window.draw(menuText);
    };

    drawText("MEGA MARIO", 75, 10, 25, 20);
    m_menuText.setPosition(m_menuText.getPosition().x, m_menuText.getPosition().y + 75);
    for (int i = 0; i < std::size(m_levels); i++) {
        auto textLevel = m_levels[i];
        if (i == m_selectedMenuIndex) {
            textLevel = textLevel + " <<<";
        }
        auto yPos = m_menuText.getPosition().y + m_menuText.getCharacterSize() + 55;
        drawText(textLevel, 60, 10, 100, yPos);
    }
    drawText("UP     DOWN   PLAY : ENTER     BACK : ESC", 25, 5, 30, (m_game->window().getSize().y - 65));
}

void Scene_Menu::sDoAction(const Action &action) {
    if (action.type() != "START") {
        return;
    }
    if (action.name() == "QUIT") m_game->quit();

    auto &i = m_selectedMenuIndex;
    if (action.name() == "UP" && i > 0) i--;
    if (action.name() == "DOWN" && i < std::size(m_levels) - 1) i++;
    if (action.name() == "PLAY") {
        m_game->changeScene("GAME",
                            std::make_shared<Scene_Zelda>(m_game, m_levels[m_selectedMenuIndex]),
                            true);
    }
}

void Scene_Menu::onEnd() {
    // TODO: when the scene ends, change back to the Menu scene use m_game -> changeScene(params)
}
