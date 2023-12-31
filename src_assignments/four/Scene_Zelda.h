#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include "EntityManager.h"
#include "Physics.h"


class Scene_Zelda : public Scene {

    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, HEALT;
        std::string WEAPON;
    };

    struct TileConfig {
        std::string NAME_ANI;
        int RX, RY, TX, TY;
        bool BM, BV;
    };

    struct NpcConfig {
        std::string NAME_ANI, AI;
        int RX, RY, TX, TY, H, D, PPN;
        bool BM, BV;
        float S;
        std::vector<Vec2> PatrolPositions;
    };

protected:
    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_plyConf;
    std::vector<Vec2> m_blackHoles;
    std::vector<TileConfig> m_tileConfig;
    std::vector<NpcConfig> m_npcConfig;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    bool m_follow = false;
    bool m_debug = false;
    const Vec2 m_gridSize = {64, 64};
    sf::Text m_gridText;
    Physics physics;

    Scene_Zelda();

    void init(const std::string &levelPath);

public:

    Scene_Zelda(GameEngine *gameEngine, const std::string &levelPath);

    void update();

    void sAnimation();

    void onEnd();

    void sMovement();

    void sStatus();

    void sCollision();

    void sRender();

    void sDoAction(const Action &action);

    void sAI();

    void sCamera();

    void spawnPlayer(Vec2 pos);

    void spawnPlayer();

    void spawnSword(std::shared_ptr<Entity> entity);

    Vec2 getPosition(int rx, int ry, int tx, int ty,std::shared_ptr<Entity> entity)const;

    Vec2 getPos(std::shared_ptr<Entity> entity, Vec2 nextPos);

    void loadLevel(const std::string &filename);

    void drawLine(const Vec2 &p1, const Vec2 &p2);

    void loadLevelConfig(const std::string &filePath);

};


