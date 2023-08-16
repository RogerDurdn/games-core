#include "Game.h"

Game::Game(const std::string &config) {
    init(config);
}


void Game::init(const std::string &config) {
    //TODO: read config, use structs for configs

    //default window parameters
    m_window.create(sf::VideoMode(800, 600), "myGame");
    m_window.setFramerateLimit(60);
    spawnPlayer();
}

void Game::setPaused(bool paused) {

}

void Game::sMovement() {

}

void Game::sUserInput() {

}

void Game::sLifeSpan() {

}

void Game::sRender() {

}

void Game::sEnemySpawner() {

}

void Game::sCollision() {

}

void Game::spawnPlayer() {

}

void Game::spawnEnemy() {

}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity) {

}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mosePos) {

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) {

}

void Game::run() {

}
