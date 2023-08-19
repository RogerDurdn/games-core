#include "Game.h"
#include <iostream>
#include <fstream>

Game::Game(const std::string &config) {
    init(config);
}

void Game::init(const std::string &config) {
    //TODO: read config, use structs for configs

    std::ifstream configFile(config);
    if(!configFile.is_open()){
        std::cerr << "Cannot open the file:"<<config<<std::endl;
        exit(-1);
    }
    std::string type;
    while (configFile >> type){
        std::cout << type << std::endl;
        if(type == "Window"){
            int w, h, r, full;
            configFile >> w >> h >> r>>full;
            m_window.create(sf::VideoMode(w, h), "myGame");
            m_window.setFramerateLimit(r);
        }
        if(type == "Font"){
            std::string path;
            int size, r, g, b;
            configFile >> path >> size >> r >> g >> b;
        }
        if (type == "Player") {
            PlayerConfig p;
            configFile >> p.SR >> p.CR >> p.S >> p.FR >> p.FG >> p.FB;
            configFile >> p.OR >> p.OG >> p.OB >> p.OT >> p.V;
            m_playerConfig = p;
        }
        if (type == "Enemy"){
            EnemyConfig e;
            configFile >> e.SR >> e.CR >> e.SMIN >> e.SMAX >> e.OR >> e.OG >> e.OB >> e.OT;
            configFile >> e.VMIN >> e.VMAX >> e.L >> e.SI;
            m_enemyConfig = e;
        }
        if (type == "Bullet") {
            BulletConfig b;
            configFile >> b.SR >> b.CR >> b.S >> b.FR >> b.FG >> b.FB >> b.OR;
            configFile >> b.OG >> b.OB >> b.OT >> b.V >> b.L;
            m_bulletConfig = b;
        }
    }
    //default window parameters
    spawnPlayer();
}


void Game::run() {

//    TODO: add pause functionality in here
// some systems should function while paused (rendering)
// some systems shouldn't (movement/input)
    while (m_running) {
        if(!m_paused){
            m_entities.update();
//        sEnemySpawner();
            sCollision();
            sMovement();
//            m_currentFrame++;
        }
        sUserInput();
        sRender();

//    increment the current frame
//    may need to be moved when pause implemented
        m_currentFrame++;
    }
}


void Game::setPaused(bool paused) {

}

void Game::sMovement() {
    // TODO: implement all entities movement here
//    you should read the m_player->cInput to determine if the player is moving

    m_player->cTransform->velocity = {0, 0};

    if (m_player->cInput->down) {
        std::cout << "down pressed\n";
        m_player->cTransform->velocity.y = m_playerConfig.S;
    }
    if (m_player->cInput->up) {
        std::cout << "up pressed\n";
        m_player->cTransform->velocity.y = -m_playerConfig.S;
    }
    if (m_player->cInput->left) {
        std::cout << "left pressed\n";
        m_player->cTransform->velocity.x = -m_playerConfig.S;
    }
    if (m_player->cInput->right) {
        std::cout << "right pressed\n";
        m_player->cTransform->velocity.x = m_playerConfig.S;
    }
// Sample movement speed update
    m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
    m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
}

void Game::sUserInput() {
    sf::Event event{};
    while (m_window.pollEvent(event)){
        switch (event.type) {
            case sf::Event::Closed:
                m_running = false;
                break;
            case sf::Event::KeyPressed:
                handlePressedReleasedKeys(event, true);
                if (event.key.code == sf::Keyboard::P) {
                    std::cout << "P key pressed\n";
                    m_paused = !m_paused;
                    break;
                }
                break;
            case sf::Event::KeyReleased:
                handlePressedReleasedKeys(event, false);
                break;
            case sf::Event::MouseButtonPressed:
                handlePressedMouseKeys(event);
                break;
        }
    }
}


void Game::handlePressedReleasedKeys(sf::Event event, bool flag){
    switch (event.key.code) {
        case sf::Keyboard::Escape:
            std::cout <<"Esc key pressed/released\n";
            exit(0);
        case sf::Keyboard::W:
            std::cout <<"W key pressed/released\n";
            m_player->cInput->up = flag;
            break;
        case sf::Keyboard::A:
            std::cout <<"a key pressed/released\n";
            m_player->cInput->left = flag;
            break;
        case sf::Keyboard::D:
            std::cout <<"d key pressed/released\n";
            m_player->cInput->right = flag;
            break;
        case sf::Keyboard::S:
            std::cout <<"s key pressed/released\n";
            m_player->cInput->down = flag;
            break;
        default:
            break;
    }
}


void Game::handlePressedMouseKeys(sf::Event event){
    switch (event.mouseButton.button) {
        case sf::Mouse::Left:
            // spawn bullet
            std::cout << "left mouse clicked at:"<<event.mouseButton.x<<","<<event.mouseButton.y<<"\n";
            spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            break;
        case sf::Mouse::Right:
            // spawn special power
            std::cout << "right mouse clicked at:"<<event.mouseButton.x<<","<<event.mouseButton.y<<"\n";
            break;
        default:
            break;
    }
}

void Game::sLifeSpan() {

}

void Game::sRender() {
    m_window.clear();
    for(auto e: m_entities.getEntities()){
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
        e->cTransform->angle += 1.5f;
        e->cShape->circle.setRotation(e->cTransform->angle);
        m_window.draw(e->cShape->circle);
    }
    m_window.display();
}

void Game::sEnemySpawner() {
    // TODO: code which implements enemy spawning should go here
    // use m_currentFrame - m_lastEnemyST to determine how long has been since the las enemy spawned
    this->spawnEnemy();
}

void Game::sCollision() {
//    TODO: implement all proper collisions between entities
//  be sure to use the collision radius, NOT the shape radius

// player collision with window bounds
    auto playerPosition = m_player->cShape->circle.getPosition();
    if (playerPosition.x < 0 || playerPosition.y < 0) {
        std::cout <<"player pos x:"<<playerPosition.x << " y:"<<playerPosition.y<< std::endl;
        m_player->cTransform->velocity = Vec2(0, 0);
    }

}

void Game::spawnPlayer() {
    auto entity = m_entities.addEntity("player");
    float wMiddleX = m_window.getSize().x/2;
    float wMiddleY = m_window.getSize().y/2;
    entity->cTransform = std::make_shared<CTransform>(Vec2(wMiddleX, wMiddleY), Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);
    entity->cShape = std::make_shared<CShape>(
            m_playerConfig.SR, m_playerConfig.V,
            sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
            sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
            m_playerConfig.OT);
    entity->cInput = std::make_shared<CInput>();
    m_player = entity;
}

void Game::spawnEnemy() {
    // TODO: make sure the enemy is spawned properly with the m_enemyconfig variables the enemy must be spawned
    //completely within the bounds of the window
    float ex = rand() % m_window.getSize().x;
    float ey = rand() % m_window.getSize().y;
    auto entity = m_entities.addEntity("enemy");

    entity->cTransform = std::make_shared<CTransform>(Vec2(ex,ey),Vec2(), 0.0f);

    entity->cShape = std::make_shared<CShape>(16.0f,3,sf::Color::Blue, sf::Color::Red, 1.0f);


    //record when the most recent enemy was spawned
    m_lastEneymySpawnTime = m_currentFrame;

}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity) {
    // TODO: spawn small enemies at the location of the input enemy e
    // when we create the smaller enemy we have to read the values of the original one
    // -- spawn a number of small enemies equal to the vertices
    // -- set each small enemy to the same color as the original, half size
    // -- small enemies are worth double points of the original

}

// spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mosePos) {
    // TODO: implement the spawning of a bullet which travels toward target
    // -- bullet speed is given as a scalar speed
    // -- you must set the velocity by using formula in notes
    auto bullet = m_entities.addEntity("bullet");
    bullet->cTransform = std::make_shared<CTransform>(mosePos, Vec2(), 0);
    bullet->cShape = std::make_shared<CShape>(2.0f,3,sf::Color::White, sf::Color::Cyan, 2);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) {

}

