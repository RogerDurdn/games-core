#pragma once
class Vec2 {
public:
    float x = 0;
    float y = 0;

    Vec2();

    Vec2(float xin, float yin);

    bool operator==(const Vec2 &rhs) const;

    bool operator!=(const Vec2 &rhs) const;

    bool operator+(const Vec2 &rhs) const;

    bool operator-(const Vec2 &rhs) const;

    bool operator/(const float val) const;

    bool operator*(const float val) const;

    bool operator+=(const Vec2 &rhs);

    bool operator-=(const Vec2 &rhs);

    bool operator*=(const float val);

    bool operator/=(const float val);

    float dist(const Vec2 &rhs) const;

};
