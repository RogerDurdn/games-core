#include "Game.h"
#include <iostream>
#include <fstream>
#include "cmath"

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
            m_font = sf::Font();
            std::string pathFile = std::getenv(path.c_str());
            m_font.loadFromFile(pathFile);
            m_text = sf::Text("Score: ",m_font, size);
            m_text.setFillColor(sf::Color(r,g,b));
            m_text.setPosition(10,10);
            m_special = sf::Text("Special: ",m_font, size-5);
            m_special.setFillColor(sf::Color(r,g,b));
            m_special.setPosition(10,50);
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
            if(m_special_cd>0)break;
            spawnSpecialWeapon(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            m_special_cd = 300;
            break;
        default:
            break;
    }
}

void Game::sLifeSpan() {
    for (auto b: m_entities.getEntities()) {
        if(b->cLifespan == nullptr){
            continue;
        }
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
    if (m_special_cd > 0)m_special_cd--;
}

void Game::sRender() {
    m_window.clear();
    m_text.setString("Score: " + std::to_string(m_score));
    m_special.setString("Special cold down: " + std::to_string(m_special_cd/60));
    m_window.draw(m_text);
    m_window.draw(m_special);
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
// collision with window bounds
    auto windowSize = Vec2(m_window.getSize().x, m_window.getSize().y);
    for (auto e: m_entities.getEntities()) {
        auto ePos = e->cTransform->pos;
        auto eRadPos = Vec2(e->cCollision->radius, e->cCollision->radius);
        auto ePosLhs = ePos - eRadPos;
        auto ePosRhs = ePos + eRadPos;
        if((ePosLhs.x > 0 && ePosLhs.y > 0) && (ePosRhs.x < windowSize.x && ePosRhs.y < windowSize.y)) continue;
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

    // bullet collisions
    for(auto b: m_entities.getEntities()){
        if(b->tag()=="bullet"||b->tag()=="special"){
            auto bulletPos = b->cTransform->pos;
            for(auto e: m_entities.getEntities()){
                if(e->tag() == "enemy" || e->tag() =="small_enemy"){
                    if(e->tag() =="small_enemy" && b->tag() =="special") continue;
                    auto distBetweenEnemyAndBullet = bulletPos.distSqr(e->cTransform->pos);
                    auto radBetweenEnemyAndBullet = (e->cCollision->radius + b->cCollision->radius);
                    if(distBetweenEnemyAndBullet < (radBetweenEnemyAndBullet * radBetweenEnemyAndBullet)){
                        e->destroy();
                        if (b->tag() == "bullet")b->destroy();
                        if (e->tag() == "enemy")this->spawnSmallEnemies(e);
                        m_score += e->cScore->score;
                    }
                }
            }
        }
    }

    // enemy - player collision
    for (auto e: m_entities.getEntities("enemy")) {
        auto ePos = e->cTransform->pos;
        auto playerDistBetween = ePos.distSqr(m_player->cTransform->pos);
        auto playerRadBetween = (e->cCollision->radius + m_player->cCollision->radius);
       if(playerDistBetween < playerRadBetween*playerRadBetween ){
           e->destroy();
           m_player->destroy();
           this->spawnPlayer();
           this->spawnSmallEnemies(e);
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
    enemy->cScore = std::make_shared<CScore>(ve*100);
    m_lastEneymySpawnTime = m_currentFrame;
}

int Game::randOf(int min, int max){
    return rand() % (max - min + 1) + min;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity) {
    int entityPoints = entity->cShape->circle.getPointCount();
    auto v = entity->cTransform->velocity;
    float degree = 360/entityPoints;
    for (size_t i = 1; i <= entityPoints; i++) {
        auto velocity = Vec2(cos(degree*i)*v.x, sin(degree*i)*v.x);
        auto smallEnemy = m_entities.addEntity("small_enemy");
        smallEnemy->cTransform = std::make_shared<CTransform>(entity->cTransform->pos,velocity, 0.0f);
        smallEnemy->cShape = std::make_shared<CShape>(m_enemyConfig.SR/2, entityPoints,
                                                 entity->cShape->circle.getFillColor(),
                                                      entity->cShape->circle.getOutlineColor(),
                                                 m_enemyConfig.OT);
        smallEnemy->cCollision = std::make_shared<CCollision>(entity->cCollision->radius);
        smallEnemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
        smallEnemy->cScore = std::make_shared<CScore>(entity->cScore->score*2);
    }
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mosePos) {
    std::cout << "bullet to:"<<mosePos.x<<","<<mosePos.y<<"\n";
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

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity, const Vec2 &mosePos) {
    auto velocity = entity->cTransform->pos.normalize(mosePos) *m_bulletConfig.S;
    auto bullet = m_entities.addEntity("special");
    bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, velocity, 30.0f);
    bullet->cShape = std::make_shared<CShape>(150,16,sf::Color::White,sf::Color::Yellow,50);
    bullet->cCollision = std::make_shared<CCollision>(150);
    bullet->cLifespan = std::make_shared<CLifespan>(60);
}

