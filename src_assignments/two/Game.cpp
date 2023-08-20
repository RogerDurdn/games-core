#include "Game.h"
#include <iostream>
#include <fstream>

Game::Game(const std::string &config) {
    init(config);
}

void Game::init(const std::string &config) {
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
    spawnPlayer();
}


void Game::run() {
    while (m_running) {
        if(!m_paused){
            m_entities.update();
            sEnemySpawner();
            sMovement();
            sCollision();
            sLifeSpan();
            m_currentFrame++;
        }
        sUserInput();
        sRender();
    }
}


void Game::setPaused(bool paused) {
    m_paused = paused;
}

void Game::sMovement() {
    Vec2 velocity(0,0);
    if (m_player->cInput->down) velocity.y = m_playerConfig.S;
    if (m_player->cInput->up) velocity.y = -m_playerConfig.S;
    if (m_player->cInput->left) velocity.x = -m_playerConfig.S;
    if (m_player->cInput->right)velocity.x = m_playerConfig.S;
    m_player->cTransform->velocity = velocity;
    for (auto e : m_entities.getEntities()){
        e->cTransform->pos += e->cTransform->velocity;
    }
}

void Game::sUserInput() {
    sf::Event event{};
    while (m_window.pollEvent(event)){
        switch (event.type) {
            case sf::Event::Closed:
                m_running = false;
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::P) {
                    this->setPaused(!m_paused);
                    break;
                }
                handlePressedReleasedKeys(event, true);
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
//    std::cout << "bullet size:"<< m_entities.getEntities("bullet").size()<<"\n";
    for (auto b: m_entities.getEntities("bullet")) {
        int total = b->cLifespan->total;
        int remaining = b->cLifespan->remaining--;
        if(remaining <= 0){
            b->destroy();
            break;
        }
        auto alpha = ((float) remaining / total) * 255;
        auto color = b->cShape->circle.getFillColor();
        auto outline = b->cShape->circle.getOutlineColor();
        b->cShape->circle.setFillColor(sf::Color(color.r, color.g, color.b, alpha));
        b->cShape->circle.setOutlineColor(sf::Color(outline.r, outline.g, outline.b, alpha));
    }
}

void Game::sRender() {
    m_window.clear();
    for(auto e: m_entities.getEntities()){
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
        e->cShape->circle.setRotation(e->cTransform->angle += 1.5);
        m_window.draw(e->cShape->circle);
    }
    m_window.display();
}

void Game::sEnemySpawner() {
    if (m_currentFrame == m_lastEneymySpawnTime + m_enemyConfig.SI){
        this->spawnEnemy();
    }
}

void Game::sCollision() {
//    TODO: implement all proper collisions between entities
//  be sure to use the collision radius, NOT the shape radius

// collision with window bounds
    auto windowSize = Vec2(m_window.getSize().x, m_window.getSize().y);
    for (auto e: m_entities.getEntities()) {
        auto ePos = e->cTransform->pos;
        auto eRadPos = Vec2(e->cCollision->radius, e->cCollision->radius);
        auto ePosLhs = ePos - eRadPos;
        auto ePosRhs = ePos + eRadPos;
        if(e->tag() =="player"){
            if (ePosLhs.x < 0) ePos.x =  eRadPos.x;
            if (ePosLhs.y < 0) ePos.y = eRadPos.y;
            if (ePosRhs.x > windowSize.x) ePos.x = windowSize.x - eRadPos.x;
            if (ePosRhs.y > windowSize.y) ePos.y = windowSize.y - eRadPos.y;
        }
        if (e->tag() == "enemy") {
            if (ePosLhs.x < 0 || ePosRhs.x > windowSize.x) ePos.x += e->cTransform->velocity.x *= -1;
            if (ePosLhs.y < 0 || ePosRhs.y > windowSize.y) ePos.y += e->cTransform->velocity.y *= -1;
        }
        e->cTransform->pos = ePos;
    }

    for (auto e: m_entities.getEntities("enemy")) {
        auto ePos = e->cTransform->pos;
        auto eRadPos = Vec2(e->cCollision->radius, e->cCollision->radius);
        auto ePosLhs = ePos - eRadPos;
        auto ePosRhs = ePos + eRadPos;
        for(auto b: m_entities.getEntities("bullet")){
            auto bPos = b->cTransform->pos;
            auto bRadPos = Vec2(b->cCollision->radius, b->cCollision->radius);
            auto bPosLhs = bPos - bRadPos;
            auto bPosRhs = bPos + bRadPos;
            if(ePosLhs.x < bPosLhs.x || ePosLhs.y < bPosLhs.y || ePosRhs.x < bPosRhs.x || ePosRhs.y < bPosRhs.y){
                e->destroy();
            }
        }
    }
}

void Game::spawnPlayer() {
    auto player = m_entities.addEntity("player");
    float wMiddleX = m_window.getSize().x/2;
    float wMiddleY = m_window.getSize().y/2;
    player->cTransform = std::make_shared<CTransform>(Vec2(wMiddleX, wMiddleY), Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);
    player->cShape = std::make_shared<CShape>(
            m_playerConfig.SR, m_playerConfig.V,
            sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
            sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
            m_playerConfig.OT);
    player->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
    player->cInput = std::make_shared<CInput>();
    m_player = player;
}

void Game::spawnEnemy() {
    int cr = m_enemyConfig.CR;
    int ex = randOf(cr, m_window.getSize().x - cr);
    int ey = randOf(cr, m_window.getSize().y - cr);
    int ve = randOf(m_enemyConfig.VMIN, m_enemyConfig.VMAX);
    auto velocity = Vec2(randOf(m_enemyConfig.SMIN, m_enemyConfig.SMAX),
                         randOf(m_enemyConfig.SMIN, m_enemyConfig.SMAX));

    auto enemy = m_entities.addEntity("enemy");
    enemy->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), velocity, 0.0f);
    enemy->cShape = std::make_shared<CShape>(m_enemyConfig.SR, ve,
                                             sf::Color(randOf(0, 255),
                                                       randOf(0, 255),
                                                       randOf(0, 255)),
                                             sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
                                             m_enemyConfig.OT);
    enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
    m_lastEneymySpawnTime = m_currentFrame;
}

int Game::randOf(int min, int max){
    return rand() % (max - min + 1) + min;
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
    std::cout << "left mouse clicked at:"<<mosePos.x<<","<<mosePos.y<<"\n";
    // TODO: implement the spawning of a bullet which travels toward target
    // -- bullet speed is given as a scalar speed
    // -- you must set the velocity by using formula in notes
//    auto speed = m_bulletConfig.S/60;
    auto velocity = entity->cTransform->pos.normalize(mosePos) *m_bulletConfig.S/2;
    auto bullet = m_entities.addEntity("bullet");
    bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, velocity, 0);
    bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR,m_bulletConfig.V,
                                              sf::Color(m_bulletConfig.FR, m_bulletConfig.FG,m_bulletConfig.FB),
                                              sf::Color(m_bulletConfig.OR, m_bulletConfig.OG,m_bulletConfig.OB),
                                              m_bulletConfig.OT);
    bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
    bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) {

}

