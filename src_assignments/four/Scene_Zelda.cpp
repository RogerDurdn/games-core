#include "Scene_Zelda.h"
#include "Scene_Menu.h"
#include "Physics.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include <fstream>
#include <sstream>

#include <iostream>

Scene_Zelda::Scene_Zelda(GameEngine *gameEngine, const std::string &levelPath)
        : Scene(gameEngine), m_levelPath(levelPath) {
    init(m_levelPath);
}

void Scene_Zelda::init(const std::string &levelPath) {
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::Y, "TOGGLE_FOLLOW");
    registerAction(sf::Keyboard::F, "TOGGLE_DEBUG");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    //player
    registerAction(sf::Keyboard::W, "PLAYER_UP");
    registerAction(sf::Keyboard::D, "PLAYER_RIGHT");
    registerAction(sf::Keyboard::A, "PLAYER_LEFT");
    registerAction(sf::Keyboard::S, "PLAYER_DOWN");
    registerAction(sf::Keyboard::Space, "PLAYER_ATTACK");

    physics = Physics();
    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Grid"));
    m_game->assets().getMusic("MusicPlay").get()->play();
    loadLevel(levelPath);
}

void Scene_Zelda::update() {
    m_entityManager.update();
    sMovement();
    sCamera();
    sStatus();
    sAI();
    sCollision();
    sAnimation();
    sRender();
}

void Scene_Zelda::sRender() {
    auto backgroundColor = sf::Color(252, 216, 166);
    m_game->window().clear(backgroundColor);

    if (m_drawTextures) {
        for (auto e: m_entityManager.getEntities()) {

            auto &transform = e->getComponent<CTransform>();
            auto &cAnimation = e->getComponent<CAnimation>();
            if (cAnimation.has) {
                auto &animation = cAnimation.animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                if (e->hasComponent<CInvincibility>()) animation.getSprite().setColor(sf::Color(255, 255, 255, 128));
                m_game->window().draw(animation.getSprite());
            }
            auto &health = e->getComponent<CHealth>();
            if (!health.has || health.current <= 0) {
                continue;
            }
            for (size_t i = 0; i < health.max; i++) {
                auto healthBarSize = Vec2(20, 4);
                auto &box = e->getComponent<CBoundingBox>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(healthBarSize.x, healthBarSize.y));
                rect.setOrigin(sf::Vector2f(healthBarSize.x * health.max / 2, box.size.y - healthBarSize.y * 2));
                rect.setPosition(transform.pos.x + healthBarSize.x * i, transform.pos.y);
                auto healthColor = i < health.current ? sf::Color::Red : sf::Color::Transparent;
                rect.setFillColor(healthColor);
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(1.0f);
                m_game->window().draw(rect);
            }
        }
    }

    if (m_drawCollision) {
        for (auto e: m_entityManager.getEntities()) {
            if (e->hasComponent<CBoundingBox>()) {
                auto &box = e->getComponent<CBoundingBox>();
                auto &transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x - 1, box.halfSize.y - 1));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color(255, 255, 255));
                if (box.blockMove && box.blockVision) {
                    rect.setOutlineColor(sf::Color::Red);
                } else if (box.blockMove) {
                    rect.setOutlineColor(sf::Color::Black);
                } else if (box.blockVision) {
                    rect.setOutlineColor(sf::Color::Blue);
                }
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    if (m_drawGrid) {
        float upY = m_game->window().getView().getCenter().y - height() / 2;
        float downY = upY + height() + m_gridSize.y;
        float leftX = m_game->window().getView().getCenter().x - width() / 2;
        float rightX = leftX + width() + m_gridSize.x;

        float nextGridX = leftX - ((int) leftX % (int) m_gridSize.x);
        for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
            drawLine(Vec2(x, upY), Vec2(x, downY));
        }

        // TODO: fix Y grid when sober
        float nextGridY = upY - ((int) upY % (int) m_gridSize.y);
        for (float y = nextGridY; y < downY; y += m_gridSize.y) {
            drawLine(Vec2(leftX, upY + y), Vec2(rightX, upY + y));
            for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
                std::string xCell = std::to_string((int) x / (int) m_gridSize.x);
                std::string yCell = std::to_string((int) y / (int) m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, y + 2);
                m_gridText.setFillColor(sf::Color::Black);
                m_game->window().draw(m_gridText);
            }
        }
    }

    if(m_debug){
        for(auto &e: m_entityManager.getEntities()){
            auto &cP = e->getComponent<CPatrol>();
            auto &cT = e->getComponent<CTransform>();
            if(cP.has){
                auto romX = 0;
                auto romY = 0;
                if (cT.pos.x < 0) romX = -1;
                if (cT.pos.x > width()) romX = 1;
                if (cT.pos.y < 0) romX = -1;
                if (cT.pos.y > height()) romX = 1;
                for(auto &pos :cP.positions){
                    auto roomPos = getPosition(romX, romY, pos.x, pos.y, e);
                    sf::CircleShape circle;
                    circle.setRadius(8);
                    circle.setPosition(sf::Vector2f(roomPos.x, roomPos.y));
                    circle.setFillColor(sf::Color::Black);
                    m_game->window().draw(circle);
                }

            }
            auto &cF = e->getComponent<CFollowPlayer>();
            if(cF.has){
                drawLine(cT.pos, m_player->getComponent<CTransform>().pos);
            }
        }
    }
}

void Scene_Zelda::sMovement() {
    auto &pT = m_player->getComponent<CTransform>();
    auto &pS = m_player->getComponent<CState>();
    if (!pT.velocity.x && !pT.velocity.y) {
        if (pS.prevState == "RunUp") pS.setState("StandU");
        if (pS.prevState == "RunRight") pS.setState("StandR");
        if (pS.prevState == "RunDown") pS.setState("StandD");
    }
    pT.velocity = Vec2();
    auto &pI = m_player->getComponent<CInput>();
    if(pI.canAttack){
        if (pI.left) pT.velocity.x -= m_plyConf.SPEED, pS.setState("RunRight"), pT.scale.x = -1;
        if (pI.right) pT.velocity.x += m_plyConf.SPEED, pS.setState("RunRight"), pT.scale.x = 1;
        if (pI.up) pT.velocity.y -= m_plyConf.SPEED, pS.setState("RunUp");
        if (pI.down) pT.velocity.y += m_plyConf.SPEED, pS.setState("RunDown");
    }
    for (auto e: m_entityManager.getEntities()) {
        auto &eT = e->getComponent<CTransform>();
        eT.prevPos = eT.pos;
        eT.pos += eT.velocity;
    }
}


void Scene_Zelda::sDoAction(const Action &action) {
    auto &player = m_player;
    auto &pI = player->getComponent<CInput>();
    auto handlePlayerInput = [&pI, this](std::string actName, bool apply) -> void {
        if (actName.find("PLAYER") != std::string::npos) {
            if (actName == "PLAYER_RIGHT") pI.right = apply;
            if (actName == "PLAYER_LEFT") pI.left = apply;
            if (actName == "PLAYER_UP") pI.up = apply;
            if (actName == "PLAYER_DOWN") pI.down = apply;
            if (actName == "PLAYER_ATTACK") {
                if (pI.canAttack) this->spawnSword(m_player), pI.canAttack = false;
            }
        }
    };

    auto actType = action.type();
    auto actName = action.name();
    if (actType == "START") {
        if (actName == "TOGGLE_TEXTURE") m_drawTextures = !m_drawTextures;
        if (actName == "TOGGLE_FOLLOW") m_follow = !m_follow;
        if (actName == "TOGGLE_DEBUG") m_debug = !m_debug;
        if (actName == "TOGGLE_COLLISION") m_drawCollision = !m_drawCollision;
        if (actName == "TOGGLE_GRID") m_drawGrid = !m_drawGrid;
        if (actName == "PAUSE") setPaused(!m_paused);
        if (actName == "QUIT") onEnd();
        handlePlayerInput(actName, true);
    }
    if (actType == "END") {
        handlePlayerInput(actName, false);
    }


}

Vec2 Scene_Zelda::getPosition(int rx, int ry, int tx, int ty, std::shared_ptr<Entity> entity) const {
    auto animSizeMiddle = entity->getComponent<CAnimation>().animation.getSize() / 2;
    auto roomX = (width() * rx) + ((m_gridSize.x * tx) + animSizeMiddle.x);
    auto roomY = (height() * ry) + ((m_gridSize.y * ty) + animSizeMiddle.y);
    return Vec2(roomX, roomY);
}


Vec2 Scene_Zelda::getPos(std::shared_ptr<Entity> entity, Vec2 nextPos) {
    auto &cT = entity->getComponent<CTransform>();
    auto romX = 0;
    auto romY = 0;
    if (cT.pos.x < 0) romX = -1;
    if (cT.pos.y < 0) romX = -1;
    if (cT.pos.x > width()) romX = 1;
    if (cT.pos.y > height()) romX = 1;
    return getPosition(romX, romY, nextPos.x, nextPos.y, entity);
}


void Scene_Zelda::sAI() {
    for (auto &npc: m_entityManager.getEntities("NPC")) {
        auto &cP = npc->getComponent<CPatrol>();
        auto &cT = npc->getComponent<CTransform>();
        if (cP.has) {
            auto nextPos = getPos(npc, cP.positions.at(cP.currentPosition));
            if(Vec2(round(cT.pos.x),round(cT.pos.y)) == nextPos){
                cP.currentPosition++;
                if(cP.currentPosition > cP.positions.size() - 1) cP.currentPosition = 0;
            } else{
                cT.velocity = cT.pos.normalize(nextPos) * cP.speed;
            }
        }

        auto &cF = npc->getComponent<CFollowPlayer>();
        if(cF.has){
            auto &pT = m_player->getComponent<CTransform>();
            auto isBlocked = false;
            for(auto &e: m_entityManager.getEntities()){
                auto &eB = e->getComponent<CBoundingBox>();
                if(!eB.blockVision)continue;

                isBlocked = physics.EntityIntersect(cT.pos, pT.pos, e);
                if(isBlocked) break;
            }
            auto vel = Vec2();
            if(!isBlocked){
                vel = cT.pos.normalize(pT.pos);
            }else if(Vec2(round(cT.pos.x),round(cT.pos.y)) != cF.home){
                vel = cT.pos.normalize(cF.home);
            }
            std::cout << "follow vel x:" << vel.x << " y:" << vel.y<<std::endl;
            cT.velocity = vel;
        }
    }
}

void Scene_Zelda::sAnimation() {
    auto &currentAnimation = m_player->getComponent<CAnimation>().animation.getName();
    auto &state = m_player->getComponent<CState>().state;
    if (state != currentAnimation) {
        auto repeat = state != "Death";
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation(state), repeat);
    }

    for (auto &e: m_entityManager.getEntities()) {
        auto &eA = e->getComponent<CAnimation>();
        if (!eA.has) continue;
        eA.animation.update();
        if (!eA.repeat && eA.animation.hasEnded()) {
            e->destroy();
            if (e->tag() == "player") spawnPlayer();
        }
    }
}

void Scene_Zelda::sStatus() {
    m_currentFrame++;
    auto &pI = m_player->getComponent<CInvincibility>();
    if (pI.has && m_currentFrame - pI.current >= pI.iframes) m_player->removeComponent<CInvincibility>();
    auto &pH = m_player->getComponent<CHealth>();
    if (pH.current <= 0) {
        m_game->assets().getSound("LinkDie").get()->play();
        m_player->getComponent<CTransform>().pos = m_player->getComponent<CTransform>().prevPos;
        m_player->removeComponent<CInput>();
        m_player->getComponent<CState>().setState("Death");
        m_player->addComponent<CInvincibility>(30, currentFrame());
    }

    for (auto &e: m_entityManager.getEntities()) {
        auto lifeSpan = e->getComponent<CLifespan>();
        if (!lifeSpan.has) continue;
        if (m_currentFrame - lifeSpan.frameCreated > lifeSpan.lifespan) {
            if (e->tag() == "sword") {
                auto &pS = m_player->getComponent<CState>();
                pS.state = pS.prevState;
                m_player->getComponent<CInput>().canAttack = true;
            }
            e->destroy();
        }
    }

    for (auto &e: m_entityManager.getEntities("NPC")) {
        auto health = e->getComponent<CHealth>();
        if (!health.has) continue;
        if (health.current == 0) {
            m_entityManager.addEntity("explosion")
                    ->with<CAnimation>(m_game->assets().getAnimation("Explosion"), false)
                    ->with<CTransform>(e->getComponent<CTransform>().pos);
            e->destroy();
            m_game->assets().getSound("EnemyDie").get()->play();
        }
    }
}

void Scene_Zelda::sCollision() {

    // move collision
    for (auto &tile: m_entityManager.getEntities()) {
        auto tB = tile->getComponent<CBoundingBox>();
        if (!tB.has || !tB.blockMove) {
            continue;
        }
        auto pOverlap = physics.GetOverlap(m_player, tile);
        if (physics.IsCollision(pOverlap)) {
            auto &pT = m_player->getComponent<CTransform>();
            if (pOverlap.x > 0 && pOverlap.y > 0) {
                pT.pos = pT.prevPos;
            }
        }
        for(auto &npc: m_entityManager.getEntities("NPC")){
            auto overlap = physics.GetOverlap(npc, tile);
            if (physics.IsCollision(overlap)) {
                auto &npcT = npc->getComponent<CTransform>();
                if (overlap.x > 0 && overlap.y > 0) {
                    npcT.pos = npcT.prevPos;
                    npcT.velocity = Vec2();
                }
            }
        }
    }

    //npc collisions
    for (auto &npc: m_entityManager.getEntities("NPC")) {
        //sword collisions
        if (!m_player->getComponent<CInput>().canAttack) {
            for (auto &sword: m_entityManager.getEntities("sword")) {
                auto overlap = physics.GetOverlap(sword, npc);
                if (physics.IsCollision(overlap)) {
                    m_game->assets().getSound("EnemyHit").get()->play();
                    auto &npcH = npc->getComponent<CHealth>();
                    npcH.current = npcH.current - sword->getComponent<CDamage>().damage;
                    sword->getComponent<CDamage>().damage = 0;
                }
            }
        }
        //player collision
        auto overlap = physics.GetOverlap(m_player, npc);
        auto &pI = m_player->getComponent<CInvincibility>();
        if (physics.IsCollision(overlap) && !pI.has) {
            m_game->assets().getSound("LinkHurt").get()->play();
            auto &pH = m_player->getComponent<CHealth>();
            pH.current = pH.current - npc->getComponent<CDamage>().damage;
            m_player->addComponent<CInvincibility>(30, m_currentFrame);
        }
    }

    // heart collisions
    for(auto &h: m_entityManager.getEntities("Heart")){
        for(auto &e: m_entityManager.getEntities()){
            auto &eH = e->getComponent<CHealth>();
            if(!eH.has)continue;
            auto overlap = physics.GetOverlap(h, e);
            if (physics.IsCollision(overlap)) {
                m_game->assets().getSound("GetItem").get()->play();
                eH.current = eH.max;
                h->destroy();
            }
        }
    }

    // Black collisions
    for(auto &b : m_entityManager.getEntities("Black")){
        auto overlap = physics.GetOverlap(b, m_player);
        if (physics.IsCollision(overlap)) {
            auto audio = m_game->assets().getSound("BlackRoom").get();
            if(audio->getStatus() != sf::Sound::Playing) audio->play();
            auto &bT = b->getComponent<CTransform>();

            int range = (m_blackHoles.size()-1) - 0 + 1;
            int num = rand() % range + 0;
            while(bT.pos == m_blackHoles.at(num)) num = rand() % range + 0;
            auto newPos = m_blackHoles.at(num) + Vec2(0,m_gridSize.y);
            m_player->destroy();
            spawnPlayer(newPos);
        }
    }
}


void Scene_Zelda::spawnPlayer() {
    m_player = m_entityManager.addEntity("player");
    auto &animation = m_game->assets().getAnimation("StandD");
    m_player->addComponent<CAnimation>(animation, true);
    auto pPos = getPosition(0, 0, m_plyConf.X, m_plyConf.Y, m_player);
    m_player->addComponent<CTransform>(pPos, Vec2(m_plyConf.SPEED, m_plyConf.SPEED), 0); // here we set the position
    m_player->addComponent<CBoundingBox>(Vec2(m_plyConf.CX, m_plyConf.CY));
    m_player->addComponent<CHealth>(m_plyConf.HEALT, m_plyConf.HEALT);
    m_player->addComponent<CState>("StandD");
    m_player->addComponent<CInput>();
}

void Scene_Zelda::spawnPlayer(Vec2 pos) {
    m_player = m_entityManager.addEntity("player");
    auto &animation = m_game->assets().getAnimation("StandD");
    m_player->addComponent<CAnimation>(animation, true);
    m_player->addComponent<CTransform>(pos, Vec2(m_plyConf.SPEED, m_plyConf.SPEED), 0); // here we set the position
    m_player->addComponent<CBoundingBox>(Vec2(m_plyConf.CX, m_plyConf.CY));
    m_player->addComponent<CHealth>(m_plyConf.HEALT, m_plyConf.HEALT);
    m_player->addComponent<CState>("StandD");
    m_player->addComponent<CInput>();
}

void Scene_Zelda::loadLevel(const std::string &filename) {
    m_entityManager = EntityManager();
    loadLevelConfig(filename);

    for (auto tile: m_tileConfig) {
        auto &animation = m_game->assets().getAnimation(tile.NAME_ANI);
        auto tileEnt = m_entityManager.addEntity(tile.NAME_ANI);
        tileEnt->addComponent<CAnimation>(animation, true);
        auto tilePos = getPosition(tile.RX, tile.RY, tile.TX, tile.TY, tileEnt);
        if(tile.NAME_ANI == "Black") m_blackHoles.push_back(tilePos);
        tileEnt->addComponent<CTransform>(tilePos);
        auto size = animation.getSize();
        tileEnt->addComponent<CBoundingBox>(size -= 15.0f, tile.BM, tile.BV);
    }

    for (auto npc: m_npcConfig) {
        auto &animation = m_game->assets().getAnimation(npc.NAME_ANI);
        auto npcEnt = m_entityManager.addEntity("NPC");
        npcEnt->addComponent<CAnimation>(animation, true);
        auto npcPos = getPosition(npc.RX, npc.RY, npc.TX, npc.TY, npcEnt);

        npcEnt->addComponent<CTransform>(npcPos);
        npcEnt->addComponent<CHealth>(npc.H, npc.H);
        npcEnt->addComponent<CDamage>(npc.D);
        npcEnt->addComponent<CBoundingBox>(animation.getSize(), npc.BM, npc.BV);
        if (npc.AI == "Patrol") {
            npcEnt->addComponent<CPatrol>(npc.PatrolPositions, npc.S);
        } else {
            npcEnt->addComponent<CFollowPlayer>(npcPos, npc.S);
        }
    }

    spawnPlayer();
}


void Scene_Zelda::sCamera() {
    auto &pPos = m_player->getComponent<CTransform>().pos;
    sf::View view = m_game->window().getView();
    auto wSize = m_game->window().getSize();
    float windowCenterX, windowCenterY;
    windowCenterX = wSize.x / 2.0f;
    windowCenterY = wSize.y / 2.0f;

    if (m_follow) {
        windowCenterX = pPos.x;
        windowCenterY = pPos.y;
    } else {
        if (pPos.x < 0) windowCenterX *= -1;
        if (pPos.y < 0) windowCenterY *= -1;
        if (pPos.x > wSize.x) windowCenterX += wSize.x;
        if (pPos.y > wSize.y) windowCenterY += wSize.y;
    }

    view.setCenter(windowCenterX, windowCenterY);
    m_game->window().setView(view);
}


void Scene_Zelda::spawnSword(std::shared_ptr<Entity> entity) {
    std::cout << "attack sword" << std::endl;
    m_game->assets().getSound("Slash")->play();

    auto &pS = entity->getComponent<CState>();
    auto &pT = entity->getComponent<CTransform>();
    auto animationName = "SwordRight";
    auto swordPos = pT.pos;
    auto swordScale = pT.scale;
    if (pS.state == "StandR" || pS.state == "RunRight") {
        swordPos.x += (m_gridSize.x * swordScale.x);
        pS.setState("AtkRight");
    }
    if (pS.state == "RunDown" || pS.state == "StandD") {
        animationName = "SwordDown";
        swordPos.y += m_gridSize.y;
        pS.setState("AtkDown");
    }
    if (pS.state == "RunUp" || pS.state == "StandU") {
        animationName = "SwordUp";
        swordPos.y -= m_gridSize.y;
        pS.setState("AtkUp");
    }

    auto sword = m_entityManager.addEntity("sword");
    auto animation = m_game->assets().getAnimation(animationName);
    sword->addComponent<CAnimation>(animation, true);
    sword->addComponent<CBoundingBox>(animation.getSize());
    sword->addComponent<CTransform>(swordPos, swordScale);
    sword->addComponent<CLifespan>(10, m_currentFrame);
    sword->addComponent<CDamage>(1);

}

void Scene_Zelda::drawLine(const Vec2 &p1, const Vec2 &p2) {
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(p1.x, p1.y), sf::Color::Black),
            sf::Vertex(sf::Vector2f(p2.x, p2.y), sf::Color::Black)
    };
    m_game->window().draw(line, 2, sf::Lines);
}


void Scene_Zelda::onEnd() {
    m_game->assets().getMusic("MusicPlay").get()->stop();
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), true);
}


void Scene_Zelda::loadLevelConfig(const std::string &fileName) {
    std::string projectPath = std::getenv("NOTLINK_ASSETS_PATH");
    std::ifstream configFile(projectPath + fileName + ".txt");
    if (!configFile.is_open()) {
        std::cerr << "Cannot open asset:" << fileName << std::endl;
        exit(-1);
    }
    std::string type, line;
    while (std::getline(configFile, line)) {
        std::istringstream iss(line);
        iss >> type;
        if (type == "Tile") {
            std::string NAME_ANI;
            int RX, RY, TX, TY;
            bool BM, BV;
            iss >> NAME_ANI >> RX >> RY >> TX >> TY >> BM >> BV;
            m_tileConfig.push_back(TileConfig{NAME_ANI, RX, RY, TX, TY, BM, BV});
        }
        if (type == "NPC") {
            std::string NAME_ANI, AI;
            int RX, RY, TX, TY, H, D, PPN, XI, YI;
            bool BM, BV;
            float S;
            iss >> NAME_ANI >> RX >> RY >> TX >> TY >> BM >> BV >> H >> D >> AI >> S;
            std::vector<Vec2> patrolPositions;
            if (AI == "Patrol") {
                iss >> PPN;
                while (iss >> XI >> YI) {
                    patrolPositions.push_back(Vec2(XI, YI));
                }
            }
            m_npcConfig.push_back(NpcConfig{NAME_ANI, AI, RX, RY, TX, TY, H, D, PPN, BM, BV, S, patrolPositions});
        }
        if (type == "Player") {
            float X, Y, CX, CY, SPEED, HEALT;
            iss >> X >> Y >> CX >> CY >> SPEED >> HEALT;
            m_plyConf = PlayerConfig{X, Y, CX, CY, SPEED, HEALT, "Sword"};
        }
    }
}

