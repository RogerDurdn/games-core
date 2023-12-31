#pragma once

#include "Entity.h"
#include "Vec2.h"
#include <SFML/Graphics.hpp>

struct Line{
    Vec2 a;
    Vec2 b;
};

struct Intersect{
    bool isIntersect = false;
    Vec2 on;
};

class Physics {
public:
    bool IsCollision(Vec2 overlap) const;

    Intersect LineIntersect(const Vec2&a, const Vec2&b, const Vec2&c, const Vec2&d);

    bool EntityIntersect(const Vec2&a, const Vec2&b, std::shared_ptr<Entity> e);

    Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

    Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};
