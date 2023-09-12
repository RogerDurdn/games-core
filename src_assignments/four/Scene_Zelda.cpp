#include "Scene_Zelda.h"
#include "Scene_Menu.h"
#include "Physics.h"
#include "Assets.h"
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
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    //player
    registerAction(sf::Keyboard::W, "PLAYER_UP");
    registerAction(sf::Keyboard::D, "PLAYER_RIGHT");
    registerAction(sf::Keyboard::A, "PLAYER_LEFT");
    registerAction(sf::Keyboard::Space, "PLAYER_SHOOT");

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Grid"));
    loadLevel(levelPath);
}

void Scene_Zelda::update() {
    m_entityManager.update();
//    sMovement();
//    sLifeSpan();
//    sCollision();
//    sAnimation();
    sRender();
}

void Scene_Zelda::sRender() {
    auto backgroundColor = sf::Color(244, 192, 127);
    // color the background darker, so you know that the game is paused
//    if (!m_paused) {
//        m_game->window().clear(sf::Color(100, 100, 255));
//    } else {
    m_game->window().clear(backgroundColor);
//    }

    auto &pPos = m_player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);

    if (m_drawTextures) {
        for (auto e: m_entityManager.getEntities()) {

            auto &transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>()) {
                auto &animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
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
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    if (m_drawGrid) {
        float leftX = m_game->window().getView().getCenter().x - width() / 2;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int) leftX % (int) m_gridSize.x);
        for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y = 0; y < height(); y += m_gridSize.y) {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));
            for (float x = nextGridX; x < rightX; x += m_gridSize.x) {
                std::string xCell = std::to_string((int) x / (int) m_gridSize.x);
                std::string yCell = std::to_string((int) y / (int) m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }

}

void Scene_Zelda::sMovement() {
    auto &pI = m_player->getComponent<CInput>();
    auto &pT = m_player->getComponent<CTransform>();
    auto &pS = m_player->getComponent<CState>();
    Vec2 velocity(0, pT.velocity.y);
//    if (pI.up && pI.canJump) {
//        velocity.y = m_plyConf.SPEED_JUMP;
//        pI.canJump = false;
//        pS.state = "jumping";
//    }
//    if (pI.left) velocity.x += -m_plyConf.SPEED_X, pT.scale = Vec2(-1, 1);
//    if (pI.right) velocity.x += m_plyConf.SPEED_X, pT.scale = Vec2(1, 1);
    pT.velocity = velocity;
    for (auto e: m_entityManager.getEntities()) {
        auto &eT = e->getComponent<CTransform>();
        if (e->tag() == "player")eT.prevPos = eT.pos;
        eT.pos += eT.velocity;
    }
    if (pS.state != "jumping") {
        pS.state = velocity.x != 0 ? "run" : "stand";
        if (velocity.y != 0) pS.state = "jumping";
    }

}


void Scene_Zelda::sDoAction(const Action &action) {
    auto scenePlay = this;
    auto &player = m_player;
    auto handlePlayerInput = [&player, &scenePlay](std::string actName, bool apply) -> void {
        auto &playerInput = player->getComponent<CInput>();
        if (actName.find("PLAYER") != std::string::npos) {
            if (actName == "PLAYER_RIGHT") playerInput.right = apply;
            if (actName == "PLAYER_LEFT") playerInput.left = apply;
            if (actName == "PLAYER_UP") playerInput.up = apply;
            if (actName == "PLAYER_SHOOT") {
                playerInput.canShoot = !apply;
            }
        }
    };

    auto actType = action.type();
    auto actName = action.name();
    if (actType == "START") {
        if (actName == "TOGGLE_TEXTURE") m_drawTextures = !m_drawTextures;
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


void Scene_Zelda::sAI(){
    /*
     * TODO: Implement Enemy AI
     * follow behavior
     * patrol behavior
     */
}

void Scene_Zelda::sAnimation() {
    /*
     * TODO implement:
     * player facing direction animation
     * sword animation based on player facing
     *  the sword should move if the player changes direction mid swing
     * destruction of entities with non-repeating finished animations
     */
    auto &currentAnimation = m_player->getComponent<CAnimation>().animation.getName();
    auto &state = m_player->getComponent<CState>().state;
    if (state != currentAnimation) {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation(state), true);
    }

    for (auto &e: m_entityManager.getEntities()) {
        auto &eA = e->getComponent<CAnimation>();
        if (!eA.has) continue;
        eA.animation.update();
        if (!eA.repeat && eA.animation.hasEnded()) e->destroy();
    }
}

void Scene_Zelda::sLifeSpan() {
    // TODO: change to sStatus
    // implement lifespan and invincibility
    m_currentFrame++;
    for (auto &e: m_entityManager.getEntities()) {
        auto lifeSpan = e->getComponent<CLifespan>();
        if (!lifeSpan.has) continue;
        if (m_currentFrame - lifeSpan.frameCreated > lifeSpan.lifespan) {
            auto &eA = e->getComponent<CAnimation>();
            if (eA.animation.getName() == "brick") {
                m_entityManager.addEntity("explosion")
                        ->with<CAnimation>(m_game->assets().getAnimation("explosion"), false)
                        ->with<CTransform>(e->getComponent<CTransform>().pos);
            }
            e->destroy();
        }
    }
}

void Scene_Zelda::sEnemySpawner() {

}

void Scene_Zelda::sCollision() {

    /*
     * TODO Implement:
     * tile collisions
     * player - enemy with max calculations
     * sword - npc
     * black tile / "teleporting"
     * entity - heart and life gain logic
     *
     * you may want to use helper functions for these behaviors or this function will get long
     */


    auto physics = Physics();
    // bullets collision
    for (auto &bullet: m_entityManager.getEntities("bullet")) {
        for (auto &tile: m_entityManager.getEntities("Tile")) {
            if (physics.IsCollision(physics.GetOverlap(bullet, tile))) {
                auto tAnim = tile->getComponent<CAnimation>().animation.getName();
                if (tAnim == "brick") tile->addComponent<CLifespan>(1, m_currentFrame);
                bullet->destroy();
            }
        }
    }

    // player collision
    auto &pT = m_player->getComponent<CTransform>();
    auto &pB = m_player->getComponent<CBoundingBox>();
    auto &pI = m_player->getComponent<CInput>();

    // player environment collision
    if ((pT.pos.x - pB.halfSize.x) < 0) pT.pos.x = pB.halfSize.x;
    if ((pT.pos.y - pB.halfSize.y) < 0) {
        pT.velocity.y = 0;
        pT.pos.y = pB.halfSize.y;
    }
    if (pT.pos.y > height()) {
        m_player->destroy();
        spawnPlayer();
    }

    for (auto &tile: m_entityManager.getEntities("Tile")) {
        auto overlap = physics.GetOverlap(m_player, tile);
        if (!physics.IsCollision(overlap))continue;
        auto &tT = tile->getComponent<CTransform>();
        auto prevOverlap = physics.GetPreviousOverlap(m_player, tile);

        if (prevOverlap.y > 0 && prevOverlap.x > 0) {
            pT.pos.x = pT.prevPos.x;
            return;
        }
        if (prevOverlap.y > 0) {
            pT.velocity.x = 0;
            bool left = tT.pos.x > pT.pos.x;
            bool right = !left;
            if (left) pT.pos.x -= overlap.x, pI.right = false;
            if (right) pT.pos.x += overlap.x, pI.left = false;
        }
        if (prevOverlap.x > 0) {
            pT.velocity.y = 0;
            bool below = tT.pos.y < pT.pos.y;
            bool above = !below;
            if (above) {
                pT.pos.y -= overlap.y;
                auto &pS = m_player->getComponent<CState>();
                if (pS.state != "run") {
                    pS.state = "stand";
                }
                m_player->getComponent<CInput>().canJump = true;
            }
            if (below) {
                pT.pos.y += overlap.y;
                auto animName = tile->getComponent<CAnimation>().animation.getName();
                if (animName == "brick") tile->addComponent<CLifespan>(1, m_currentFrame);
                if (animName == "question") {
                    tile->addComponent<CAnimation>(m_game->assets().getAnimation("questionDone"), true);
                    auto coin = m_entityManager.addEntity("coin");
                    coin->addComponent<CTransform>(Vec2(tT.pos.x, tT.pos.y - m_gridSize.y));
                    coin->addComponent<CAnimation>(m_game->assets().getAnimation("coin"), false);
                }
            }
        }
    }
}


void Scene_Zelda::sDebug() {

}

void Scene_Zelda::spawnPlayer() {
    m_player = m_entityManager.addEntity("player");
    auto &animation = m_game->assets().getAnimation("StandD");
    m_player->addComponent<CAnimation>(animation, true);
    m_player->addComponent<CTransform>(Vec2(m_plyConf.X, m_plyConf.Y),
            Vec2(m_plyConf.SPEED, m_plyConf.SPEED), 0); // here we set the position
    m_player->addComponent<CBoundingBox>(Vec2(m_plyConf.CX, m_plyConf.CY));
    m_player->addComponent<CHealth>(m_plyConf.HEALT, m_plyConf.HEALT);
    m_player->addComponent<CState>();
    m_player->addComponent<CInput>();
}


Vec2 Scene_Zelda::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity) {
    auto animSizeMiddle = entity->getComponent<CAnimation>().animation.getSize() / 2;
    auto entityGx = (m_gridSize.x * gridX) + animSizeMiddle.x;
    auto entityGy = (height() - m_gridSize.y * gridY) - animSizeMiddle.y;
    return Vec2(entityGx, entityGy);
}

void Scene_Zelda::loadLevel(const std::string &filename) {
    // TODO: use getPosition middle
    // all are squares with 64x64 size
    m_entityManager = EntityManager();
    m_entityManager = EntityManager{};
    loadLevelConfig(filename);

    for (auto misc: m_tileConfig) {
//        auto &animation = m_game->assets().getAnimation(misc.NAME_ANI);
//        auto miscEntity = m_entityManager.addEntity(misc.TYPE);
//        miscEntity->addComponent<CAnimation>(animation, true);
//        auto pos = gridToMidPixel(misc.GX, misc.GY, miscEntity);
//        miscEntity->addComponent<CTransform>(pos);
//        if (misc.TYPE != "Dec") {
//            miscEntity->getComponent<CTransform>().prevPos = pos;
//            miscEntity->addComponent<CBoundingBox>(animation.getSize());
//        }
    }
    spawnPlayer();
}


Vec2 Scene_Zelda::getPosition(int rx, int ry, int tx, int ty)const{
    // TODO: Implement that takes room (rx,ry) coordinates
    // as well as teh tile (tx,ty) coordinate, an returns the Vec2 game world
    // position of the center of the entity.
    return Vec2();
}


void Scene_Zelda::sCamera() {
/*
 * TODO:
 * camera view logic
 * get teh current view, which we sill modify in the if-statement below
 */
sf::View view = m_game->window().getView();

if (m_follow){
    // calculate view for player follow camera
}else{
    // calculate view for room-based camera
}

// then set the window view
m_game->window().setView(view);
}


void Scene_Zelda::spawnSword(std::shared_ptr<Entity> entity){
    /*
     * TODO:
     * Implement the spawning of the sword, which:
     * - should be given the appropriate lifespan
     * - should spawn at the appropriate location based on player's facing direction
     * - be given a max value of 1
     * - should play the "Slash" sound
     */
}

void Scene_Zelda::drawLine(const Vec2 &p1, const Vec2 &p2) {
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(p1.x, p1.y)),
            sf::Vertex(sf::Vector2f(p2.x, p2.y))
    };
    m_game->window().draw(line, 2, sf::Lines);
}


void Scene_Zelda::onEnd() {
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
        std::istringstream  iss(line);
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
            if(AI == "Patrol"){
                while (iss >> XI >> YI) {
                    patrolPositions.push_back(Vec2(XI,YI));
                }
            }
            m_npcConfig.push_back(NpcConfig{NAME_ANI,AI,RX,RY,TX,TY,H,D,PPN,BM,BV,S,patrolPositions});
        }
        if (type == "Player") {
            float X, Y, CX, CY, SPEED, HEALT;
            iss >> X >> Y >> CX >> CY >> SPEED >> HEALT;
            m_plyConf = PlayerConfig{X, Y, CX, CY, SPEED, HEALT, "Sword"};
        }
    }
}

