#pragma once

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Animation.h"

class Assets {
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, Animation> m_animations;
    std::map<std::string, sf::Sound> m_sounds;
    std::map<std::string, sf::Font> m_fonts;

public:
    Assets();

    void loadFromFile(const std::string &path);

    void addTexture(const std::string &name, const std::string &path) const;

    void addAnimation(const std::string &name, Animation animation) const;

    void addSound(const std::string &name, const std::string &path) const;

    void addFont(const std::string &name, const std::string &path) const;

    const sf::Texture &getTexture(const std::string &name) const;

    const Animation getAnimation(const std::string &name) const;

    const sf::Sound &getSound(const std::string &name) const;

    const sf::Font &getFont(const std::string &name) const;
};
