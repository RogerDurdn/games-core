#include "Vec2.h"
#include <cmath>

Vec2::Vec2(){}

Vec2::Vec2(float xin, float yin):x(xin), y(yin) {}

bool Vec2::operator==(const Vec2 &rhs) const {
    return rhs.x == x && rhs.y == y;
}

bool Vec2::operator!=(const Vec2 &rhs) const {
    return !operator==(rhs);
}

Vec2 Vec2::operator+(const Vec2 &rhs) const {
    return Vec2(x+rhs.x, y+rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const {
    return Vec2(x-rhs.x,y-rhs.y);
}

Vec2 Vec2::operator/(const float val) const {
    return Vec2(x/val,y/val);
}

Vec2 Vec2::operator*(const float val) const {
    return Vec2(x*val,y*val);
}

void Vec2::operator+=(const Vec2 &rhs) {
   x+=rhs.x;
   y+=rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs) {
    x-=rhs.x;
    y-=rhs.y;
}

void Vec2::operator*=(const float val) {
    x *= val;
    y *= val;
}

void Vec2::operator/=(const float val) {
    x /= val;
    y /= val;
}

// TODO: testing
float Vec2::distSqr(const Vec2 &rhs) const {
    Vec2 d(rhs.x - x, rhs.y - y);
    return (d.x * d.x) + (d.y * d.y);
}

float Vec2::dist(const Vec2 &rhs) const {
    return std::sqrt(this->distSqr(rhs));
}