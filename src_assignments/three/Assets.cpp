#include "Assets.h"
#include <fstream>

Assets::Assets() {}

void Assets::loadFromFile(const std::string &path) {
    std::ifstream configFile(path);
    if (!configFile.is_open()) {
        std::cerr << "Cannot open the file:" << path << std::endl;
        exit(-1);
    }
    std::string type;
    while (configFile >> type) {
        std::cout << type << std::endl;
        if (type == "Font") {
            std::string name, pathFont;
            configFile >> name >> pathFont;
            auto aFont = sf::Font();
            std::string pathFile = std::getenv(pathFont.c_str());
            aFont.loadFromFile(pathFile);
            m_fonts[name] = aFont;
        }
    }

}

void Assets::addTexture(const std::string &name, const std::string &path) const {

}

void Assets::addAnimation(const std::string &name, Animation animation) const {

}

void Assets::addFont(const std::string &name, const std::string &path) const {

}


const sf::Texture &Assets::getTexture(const std::string &name) const {
    return m_textures.at(name);
}

const Animation Assets::getAnimation(const std::string &name) const {
    return m_animations.at(name);
}

const sf::Font &Assets::getFont(const std::string &name) const {
    return m_fonts.at(name);
}


void Assets::addSound(const std::string &name, const std::string &path) const {

}


const sf::Sound &Assets::getSound(const std::string &name) const {
    return m_sounds.at(name);
}

