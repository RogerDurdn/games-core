#include "Assets.h"

Assets::Assets() {

}

void Assets::loadFromFile(const std::string &path) {

}

void Assets::addTexture(const std::string &name, const std::string &path) const {

}

void Assets::addAnimation(const std::string &name, Animation animation) const {

}

void Assets::addSound(const std::string &name, const std::string &path) const {

}

void Assets::addFont(const std::string &name, const std::string &path) const {

}

const sf::Texture &Assets::getTexture(const std::string &name) const {
    return m_textures.at(name);
}

const Animation Assets::getAnimation(const std::string &name) const {
    return m_animations.at(name);
}

const sf::Sound &Assets::getSound(const std::string &name) const {
    return m_sounds.at(name);
}

const sf::Font &Assets::getFont(const std::string &name) const {
    return m_fonts.at(name);
}
