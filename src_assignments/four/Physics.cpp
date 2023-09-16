#include "Physics.h"
#include "Components.h"
#include <math.h>

/*
 * Center AABB overlap intersection
 */
Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    auto aPos = a->getComponent<CTransform>().pos;
    auto bPos = b->getComponent<CTransform>().pos;
    auto aHalfSize = a->getComponent<CBoundingBox>().halfSize;
    auto bHalfSize = b->getComponent<CBoundingBox>().halfSize;

    auto delta = Vec2(std::abs(aPos.x - bPos.x), std::abs(aPos.y - bPos.y));
    auto overlapX = aHalfSize.x + bHalfSize.x - delta.x;
    auto overlapY = aHalfSize.y + bHalfSize.y - delta.y;
    return Vec2(overlapX, overlapY);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
    auto aPos = a->getComponent<CTransform>().prevPos;
    auto bPos = b->getComponent<CTransform>().pos;
    auto aHalfSize = a->getComponent<CBoundingBox>().halfSize;
    auto bHalfSize = b->getComponent<CBoundingBox>().halfSize;

    auto delta = Vec2(std::abs(bPos.x - aPos.x), std::abs(bPos.y - aPos.y));
    auto overlapX = aHalfSize.x + bHalfSize.x - delta.x;
    auto overlapY = aHalfSize.y + bHalfSize.y - delta.y;
    return Vec2(overlapX, overlapY);
}

bool Physics::IsCollision(Vec2 overlap) const {
    return overlap.x > 0 && overlap.y > 0;
}

float crossProduct(const Vec2 &a, const Vec2 &b){
    return (a.x*b.y) - (a.y*b.x);
}

Intersect Physics::LineIntersect(const Vec2&a, const Vec2&b, const Vec2&c, const Vec2&d){
    Vec2 r = b-a;
    Vec2 s = d-c;
    float rXs = crossProduct(r, s);
    Vec2 cma = c-a;
    float t = crossProduct(cma, s) / rXs;
    float u = crossProduct(cma, r) / rXs;

    if ((t >= 0 && t <= 1) && (u >= 0 && u <= 1)) {
        return Intersect{true, Vec2(a.x +t*r.x, a.y+t*r.y)};
    }
    return Intersect();
}

bool Physics::EntityIntersect(const Vec2&a, const Vec2&b, std::shared_ptr<Entity> e){
    auto &eT = e->getComponent<CTransform>();
    auto &eB = e->getComponent<CBoundingBox>();
    auto upperLeftPos = Vec2(eT.pos.x - eB.halfSize.x, eT.pos.y - eB.halfSize.y);

    auto topLine = Line{upperLeftPos, Vec2(upperLeftPos.x + eB.size.x, upperLeftPos.y)};
    auto bottomLine = Line{Vec2(topLine.a.x, topLine.a.y+eB.size.y), Vec2(topLine.b.x, topLine.b.y+eB.size.y)};
    auto leftLine = Line{topLine.a, bottomLine.a};
    auto rightLine = Line{topLine.b, bottomLine.b};
    Line lines[4] ={leftLine, rightLine, topLine,bottomLine};

    for(auto &line: lines){
        auto intersect  = LineIntersect(a, b, line.a, line.b);
        if(intersect.isIntersect) return true;
    }
    return false;
}
