#include "Assets.h"
#include <fstream>
#include <string>


Assets::Assets() {}

void handleLoad(bool loaded, std::string msg) {
    if (!loaded) {
        std::cerr << "Cannot open asset:" << msg << std::endl;
        exit(-1);
    }
}

void Assets::loadFromFile(const std::string &path) {
    std::ifstream configFile(path + "config.txt");
    handleLoad(configFile.is_open(), path);
    std::string type;
    while (configFile >> type) {
        std::cout << type << std::endl;
        if (type == "Font") {
            std::string name, pathFont;
            configFile >> name >> pathFont;
            auto aFont = sf::Font();
            handleLoad(aFont.loadFromFile(path + pathFont.c_str()), pathFont);
            m_fonts[name] = aFont;
        }
        if (type == "Texture") {
            std::string name, pathTex;
            configFile >> name >> pathTex;
            sf::Texture texture;
            handleLoad(texture.loadFromFile(path + pathTex.c_str()), pathTex);
            m_textures[name] = texture;
        }
        if (type == "Animation") {
            std::string name, texName;
            int frameCount, speed;
            configFile >> name >> texName >> frameCount >> speed;
            m_animations[name] = Animation(name, m_textures[texName], frameCount, speed);
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

