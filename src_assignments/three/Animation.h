#pragma once

#include <SFML/Graphics.hpp>
#include "Vec2.h"

class Animation {
    sf::Sprite sprite;
    int frameCount;
    int currentFrame;
    int speed;
    Vec2 size;
    std::string name;

public:
    void update() const;

    void hasEnded() const;

    std::string &getName();

    Vec2 getSize();

    sf::Sprite &getSpriete();
};