#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>
#include "Animation.h"

class Component {
public:
    bool has = false;
};

class CTransform : public Component {
public:
    Vec2 pos = {0.0, 0.0};
    Vec2 prevPos = {0.0, 0.0};
    Vec2 scale = {1.0, 1.0};
    Vec2 velocity = {0.0, 0.0};
    float angle = 0;

    CTransform() {}

    CTransform(const Vec2 &p)
            : pos(p) {
    }

    CTransform(const Vec2 &p, const Vec2 &v, float a)
            : pos(p), velocity(v), angle(a) {
    }
};


class CLifespan : public Component {
public:
    int lifespan = 0; // amount of lifespan on the entity
    int frameCreated = 0; // the frameCreated initial amount of lifespan
    CLifespan() {}

    CLifespan(int duration, int frame) : frameCreated(frame), lifespan(duration) {}
};


class CInput : public Component {
public:
    bool up = false;
    bool left = false;
    bool right = false;
    bool down = false;
    bool shoot = false;
    bool canShoot = true;
    bool canJump = true;

    CInput() {}
};

class CBoundingBox : public Component {
public:
    Vec2 size;
    Vec2 halfSize;
    bool blockMove = false;
    bool blockVision = false;

    CBoundingBox() {}

    CBoundingBox(const Vec2 &s)
            : size(s), halfSize(s.x / 2, s.y / 2) {}
    CBoundingBox(const Vec2 &s, bool m, bool v)
            : size(s), halfSize(s.x / 2, s.y / 2), blockMove(m), blockVision(v) {}
};

class CAnimation : public Component {
public:
    Animation animation;
    bool repeat = false;
    bool idle = false;

    CAnimation() {}

    CAnimation(const Animation &a, bool show)
            : animation(a), repeat(show) {
    }

};

class CGravity : public Component {
public:
    float gravity = 0;

    CGravity() {}

    CGravity(float g) : gravity(g) {}

};

class CState : public Component {
public:
    std::string state = "jumping";

    CState() {}

    CState(const std::string &s) : state(s) {}
};


class CDraggable : public Component {

public:
    bool isDragged = false;

    CDraggable() {}
};


class CDamage: public Component{
public:
    int damage = 1;
    CDamage(){}
    CDamage(int d ): damage(d){}
};

class CInvincibility: public Component{
public:
    int iframes = 0;
    CInvincibility(){}
    CInvincibility(int d ): iframes(d){}
};

class CHealth: public Component{
public:
    int max = 1;
    int current = 1;
    CHealth(){}
    CHealth(int d , int c): max(d), current(c){}
};

class CFollowPlayer: public Component{
public:
    Vec2 home = {0,0};
    float speed = 0;
    CFollowPlayer(){}
    CFollowPlayer(Vec2 p, float s ): home(p), speed(s){}
};

class CPatrol: public Component{
public:
    std::vector<Vec2> positions;
    size_t currentPosition = 0;
    float speed = 0;
    CPatrol(){}
    CPatrol(std::vector<Vec2> pos, float s): positions(pos), speed(s){}
};
