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
