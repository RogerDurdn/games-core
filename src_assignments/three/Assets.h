#pragma once

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animation.h"

class Assets {
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, Animation> animations;
    std::map<std::string, sf::Sound> sounds;
    std::map<std::string, sf::Font> fonts;

public:

    void addTexture(std::string name, std::string path) const;

    void addAnimation(std::string name, Animation animation) const;

    void addSound(std::string name, std::string path) const;

    void addFont(std::string name, std::string path) const;

    sf::Texture &getTexture(std::string name) const;

    Animation &getAnimation(std::string name) const;

    sf::Sound &getSound(std::string name) const;

    sf::Font &getFont(std::string name) const;
};
