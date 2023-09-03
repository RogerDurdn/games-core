#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include "EntityManager.h"


class Scene_Play : public Scene {

    struct PlayerConfig {
        //    2    3  48  48     5          -20        20       0.75
        float GX, GY, CW, CH, SPEED_X, SPEED_JUMP, SPEED_MAX, GRAVITY;
        std::string WEAPON;// Buster
    };

    struct MiscConfig {
        std::string TYPE, NAME_ANI;
        float GX, GY;
    };

protected:
    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_plyConf;
    std::vector<MiscConfig> m_miscConfig;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    const Vec2 m_gridSize = {64, 64};
    sf::Text m_gridText;

    Scene_Play();

    void init(const std::string &levelPath);

public:

    Scene_Play(GameEngine *gameEngine, const std::string &levelPath);

    void update();

    // Systems
    void sAnimation();

    void onEnd();

    void sMovement();

    void sEnemySpawner();

    void sLifeSpan();

    void sCollision();

    void sRender();

    void sDoAction(const Action &action);

    void sDebug();

    void spawnPlayer();

    void spawnBullet(std::shared_ptr<Entity> entity);

    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

    void loadLevel(const std::string &filename);

    void drawLine(const Vec2 &p1, const Vec2 &p2);

    void loadLevelConfig(const std::string &filePath);
};


