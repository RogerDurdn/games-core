#include "Scene_Play.h"
#include "Scene_Menu.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Component.h"
#include "Action.h"
#include <fstream>

#include <iostream>

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
        : Scene(gameEngine), m_levelPath(levelPath) {
    init(m_levelPath);
}

void Scene_Play::init(const std::string &levelPath) {
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    //player
    registerAction(sf::Keyboard::W, "PLAYER_UP");
    registerAction(sf::Keyboard::D, "PLAYER_RIGHT");
    registerAction(sf::Keyboard::A, "PLAYER_LEFT");
    registerAction(sf::Keyboard::Space, "PLAYER_SHOOT");

    // TODO: Register all other gameplay Actions
    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Grid"));
    loadLevel(levelPath);
}

void Scene_Play::update() {
    m_entityManager.update();
    // TODO: implement pause functionality
    sMovement();
    sLifeSpan();
    sCollision();
    sAnimation();
    sRender();
}

void Scene_Play::sRender() {
    auto backgroundColor = sf::Color(4, 156, 216);
    // color the background darker, so you know that the game is paused
//    if (!m_paused) {
//        m_game->window().clear(sf::Color(100, 100, 255));
//    } else {
    m_game->window().clear(backgroundColor);
//    }

    //set the viewport of the window to be centered on the player if it's far enough right
    auto &pPos = m_player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);

    // draw all entity textures / animations

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
    // draw all entity collision bounding boxes with a rectangle shape
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

    // draw the grid so debug can be easy
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

void Scene_Play::sMovement() {
    /*
     * TODO: Implement player movement / jumping based on its CInput component
     * TODO: Implement gravity's effect on the player
     * TODO: Implement the maximum player speed in both GX and GY directions
     * TODO: Setting and entity's scale.x to -1/1 will make it face to the left/right
     */
    // TODO: if the player is moving faster than max speed in any direction, set its speed in that direction to the max speed
    auto &pI = m_player->getComponent<CInput>();
    auto &pT = m_player->getComponent<CTransform>();
    auto &pS = m_player->getComponent<CState>();
    Vec2 velocity(0, pT.velocity.y);
    if (pI.up && pI.canJump) {
        velocity.y = m_plyConf.SPEED_JUMP;
        pI.canJump = false;
        pS.state = "jumping";
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Jump"), true);
    }
    if (pI.left) velocity.x += -m_plyConf.SPEED_X, pT.scale = Vec2(-1, 1);
    if (pI.right) velocity.x += m_plyConf.SPEED_X, pT.scale = Vec2(1, 1);
    pT.velocity = velocity;
    for (auto e: m_entityManager.getEntities()) {
        auto &eG = e->getComponent<CGravity>();
        auto &eT = e->getComponent<CTransform>();
        if (eG.has) eT.velocity.y += eG.gravity;
        if (e->tag() == "player")eT.prevPos = eT.pos;
        eT.pos += eT.velocity;
    }
    if (pS.state != "jumping") {
        if (velocity.x != 0) {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Run"), true);
            pS.state = "running";
        }
        if (pT.pos.x == pT.prevPos.x)m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
    }

}


void Scene_Play::sDoAction(const Action &action) {
    auto scenePlay = this;
    auto &player = m_player;
    auto handlePlayerInput = [&player, &scenePlay](std::string actName, bool apply) -> void {
        auto &playerInput = player->getComponent<CInput>();
        if (actName.find("PLAYER") != std::string::npos) {
            if (actName == "PLAYER_RIGHT") playerInput.right = apply;
            if (actName == "PLAYER_LEFT") playerInput.left = apply;
            if (actName == "PLAYER_UP") playerInput.up = apply;
            if (actName == "PLAYER_SHOOT") {
                if (apply && playerInput.canShoot) scenePlay->spawnBullet(player);
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
        //player
        handlePlayerInput(actName, true);
    }
    if (actType == "END") {
        handlePlayerInput(actName, false);
    }
}

void Scene_Play::sAnimation() {
// TODO: complete te animation class code first

/*
 * TODO: set the animation of the player based on its CState component
 * TODO: for each entity with animation, call entity->get;component<CAnimation>().animation.update()
 * if the animation is not repeated, and it has ended, destroy the entity.
 */
}

void Scene_Play::sLifeSpan() {
    m_currentFrame++;
    for (auto &e: m_entityManager.getEntities()) {
        auto lifeSpan = e->getComponent<CLifespan>();
        if (!lifeSpan.has) continue;
        if (m_currentFrame - lifeSpan.frameCreated > lifeSpan.lifespan) e->destroy();
    }
    // TODO: check lifespan of entities that have they, and destroy them if they go over
}

void Scene_Play::sEnemySpawner() {

}

void Scene_Play::sCollision() {
    /* REMEMBER: SFML (0,0) position is on the TOP-LEFT center
     * This means jumping will have a negative y-component
     * and gravity will have a positive y-component
     * Also, something BELOW something else will have a y value GREATER than it
     * Also, something ABOVE something else will have a y value LESS than it
      */

    // TODO: Implement Physic::GetOverlap() function, use it inside this function
    /*
     * TODO: implement bullet / tile collisions
     * destroy the tile if it has a Brick animation
     * TODO: implement player / tile collisions and resolutions
     * Update the CState component of the player to store whether
     * it is currently on th ground or in the air. This will be used by the Animation system
     * TODO: Check to see if the player has fallen down a hole (y > height())
     * TODO: Don't let the player walk off the left side of the map
     */
    auto physics = Physics();
    // bullets collision
    for (auto &bullet: m_entityManager.getEntities("bullet")) {
        for (auto &tile: m_entityManager.getEntities("Tile")) {
            if (physics.IsCollision(physics.GetOverlap(bullet, tile))) {
                auto tAnim = tile->getComponent<CAnimation>().animation.getName();
                if (tAnim == "Brick") tile->destroy();
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

    // player elements collision
    for (auto &tile: m_entityManager.getEntities("Tile")) {
        auto overlap = physics.GetOverlap(m_player, tile);
        if (!physics.IsCollision(overlap))continue;
        auto &tT = tile->getComponent<CTransform>();
        auto prevOverlap = physics.GetPreviousOverlap(m_player, tile);

        std::cout << overlap.x << " with y:" << overlap.y << std::endl;
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
                if (pS.state != "running") {
                    pS.state = "stand";
                    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
                }
                m_player->getComponent<CInput>().canJump = true;
            }
            if (below) {
                pT.pos.y += overlap.y;
                if (tile->getComponent<CAnimation>().animation.getName() == "Brick") {
                    tile->destroy();
                }
            }
        }
    }
}


void Scene_Play::sDebug() {

}

void Scene_Play::spawnPlayer() {

    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    auto &animation = m_game->assets().getAnimation("Stand");
    m_player->addComponent<CAnimation>(animation, true);
    m_player->addComponent<CTransform>(
            gridToMidPixel(m_plyConf.GX, m_plyConf.GY, m_player),
            Vec2(m_plyConf.SPEED_X, 0), 0); // here we set the position
    m_player->addComponent<CBoundingBox>(Vec2(m_plyConf.CW, m_plyConf.CH));
    m_player->addComponent<CGravity>(m_plyConf.GRAVITY);
    m_player->addComponent<CState>();
    m_player->addComponent<CInput>();

    //TODO: be sure to add the lifespan components to the player
}


void Scene_Play::spawnBullet(std::shared_ptr<Entity> entity) {
    auto bulletSpeed = 10.0f;
    auto bulletDuration = 60;
    auto &pT = entity->getComponent<CTransform>();
    auto &pB = entity->getComponent<CBoundingBox>();
    auto origin = Vec2(pT.pos.x + (pB.halfSize.x * pT.scale.x), pT.pos.y);
    std::cout << "bullet from:" << origin.x << "," << origin.y << "\n";
    auto velocity = pT.pos.normalize(Vec2(pT.pos.x + pT.scale.x * m_plyConf.GX, pT.pos.y)) * bulletSpeed;

    auto bullet = m_entityManager.addEntity("bullet");
    bullet->addComponent<CAnimation>(m_game->assets().getAnimation(m_plyConf.WEAPON), true);
    bullet->addComponent<CTransform>(origin, velocity, 1);
    bullet->addComponent<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize());
    bullet->addComponent<CLifespan>(bulletDuration, m_currentFrame);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity) {
    /* TODO:
     * This function takes in a grid (x,y) position and a Entity
     * Return a Vec2 indicating where the CENTER position of the Entity should be
     * You must use the Entity's animation size to position it correctly
     * The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
     * The bottom-left corner of the animation should align with the bottom left of the grid cell
     */

    auto animSizeMiddle = entity->getComponent<CAnimation>().animation.getSize() / 2;
    auto entityGx = (m_gridSize.x * gridX) + animSizeMiddle.x;
    auto entityGy = (height() - m_gridSize.y * gridY) - animSizeMiddle.y;
    return Vec2(entityGx, entityGy);
}

/*
 *  TODO: read in the level file and add the appropriate entities
 *  use the playerConfig struct m_pl to store player properties
 *  this struct is defined at the top of Scene_Play.h
 *
 *  Note all of the code bellow is sample which show you how to
 *  set up and use entities with the new syntax, it should be removed
 */
void Scene_Play::loadLevel(const std::string &filename) {
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();
    m_entityManager = EntityManager{};
    loadLevelConfig(filename);

    for (auto misc: m_miscConfig) {
        auto &animation = m_game->assets().getAnimation(misc.NAME_ANI);
        auto miscEntity = m_entityManager.addEntity(misc.TYPE);
        miscEntity->addComponent<CAnimation>(animation, true);
        auto pos = gridToMidPixel(misc.GX, misc.GY, miscEntity);
        miscEntity->addComponent<CTransform>(pos);
        if (misc.TYPE != "Dec") {
            miscEntity->getComponent<CTransform>().prevPos = pos;
            miscEntity->addComponent<CBoundingBox>(animation.getSize());
        }
    }
    spawnPlayer();

    /*
     * NOTE: This is IMPORTANT - READ
     * components are now returned as references rather than pointers
     * if you do not specify a reference variable type, it will COPY the component
     * example:
     *
     * this will copy the transform into the variable 'transform1' - it is INCORRECT
     * any changes you make to transform1 will not be changed inside the entity
     * > auto transform1 = entity -> get<CTransform>()
     *
     * this will REFERENCE the transform with the variable 'transform2' - it is correct
     * now any changes you make to transform2 will be changed inside the entity
     * > auto & transform1 = entity -> get<CTransform>()
     */
}

void Scene_Play::drawLine(const Vec2 &p1, const Vec2 &p2) {
    sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(p1.x, p1.y)),
            sf::Vertex(sf::Vector2f(p2.x, p2.y))
    };
    m_game->window().draw(line, 2, sf::Lines);
}


void Scene_Play::onEnd() {
    // TODO: when the scene ends, change back to the Menu scene use m_game -> changeScene(params)
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game), true);
}


void Scene_Play::loadLevelConfig(const std::string &fileName) {
    std::string projectPath = std::getenv("MEGA_ASSETS_PATH");
    std::ifstream configFile(projectPath + "/config/" + fileName + ".txt");
    if (!configFile.is_open()) {
        std::cerr << "Cannot open asset:" << fileName << std::endl;
        exit(-1);
    }
    std::string type;
    while (configFile >> type) {
        std::cout << type << std::endl;
        if (type == "Tile" || type == "Dec") {
            std::string animationName;
            float GX, GY;
            configFile >> animationName >> GX >> GY;
            m_miscConfig.push_back(MiscConfig{type, animationName, GX, GY});
        }
        if (type == "Player") {
            float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
            std::string WEAPON;
            configFile >> X >> Y >> CX >> CY >> SPEED >> MAXSPEED >> JUMP >> GRAVITY >> WEAPON;
            m_plyConf = PlayerConfig{X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY, WEAPON};
        }
    }
}

