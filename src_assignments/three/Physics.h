#pragma once

#include "Entity.h"
#include "Vec2.h"
#include <SFML/Graphics.hpp>

typedef std::string Line;

class Physics {
public:
    bool IsCollision(const Entity &, Entity &) const;

    bool IsIntersect(Line, Line) const;

    bool IsInside(Vec2, Line) const;
};