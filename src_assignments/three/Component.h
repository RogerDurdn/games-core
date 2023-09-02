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

    CTransform();

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

class CCollision : public Component {
public:
    float radius = 0;

    CCollision(float r) : radius(r) {}
};

class CBoundingBox : public Component {
public:
    Vec2 size;
    Vec2 halfSize;

    CBoundingBox() {}

    CBoundingBox(const Vec2 &s)
            : size(s), halfSize(s.x / 2, s.y / 2) {}
};

class CAnimation : public Component {
public:
    Animation animation;
    bool repeat = false;

    CAnimation() {}

    CAnimation(Animation &a, bool show)
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
