#include "Vec2.h"
#include "math.h"

Vec2::Vec2(){

}

Vec2::Vec2(float xin, float yin):x(xin), y(yin) {
}

bool Vec2::operator==(const Vec2 &rhs) const {
    return false;
}

bool Vec2::operator!=(const Vec2 &rhs) const {
    return false;
}

Vec2 Vec2::operator+(const Vec2 &rhs) const {
    return Vec2(this->x+rhs.x, this->y+rhs.y);
}

bool Vec2::operator-(const Vec2 &rhs) const {
    return false;
}

bool Vec2::operator/(const float val) const {
    return false;
}

bool Vec2::operator*(const float val) const {
    return false;
}

bool Vec2::operator+=(const Vec2 &rhs) {
    return false;
}

bool Vec2::operator-=(const Vec2 &rhs) {
    return false;
}

bool Vec2::operator*=(const float val) {
    return false;
}

bool Vec2::operator/=(const float val) {
    return false;
}

float Vec2::dist(const Vec2 &rhs) const {
    return 0;
}


