#pragma once

#include "Entity.h"
#include "Vec2.h"
#include <SFML/Graphics.hpp>

typedef std::string Line;

class Physics {
public:
    bool IsCollision(Vec2 overlap) const;

    bool IsIntersect(Line, Line) const;

    bool IsInside(Vec2, Line) const;

    Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

    Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};
