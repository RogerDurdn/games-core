#include "Animation.h"
#include <cmath>


Animation::Animation() {

}

Animation::Animation(const std::string &name, const sf::Texture &texture)
        : Animation(name, texture, 1, 0) {
}

Animation::Animation(const std::string &name, const sf::Texture &texture, size_t frameCount, size_t speed)
        : m_name(name), m_sprite(texture), m_frameCount(frameCount), m_currentFrame(0), m_speed(speed) {
    m_size = Vec2((float) texture.getSize().x / frameCount, (float) texture.getSize().y);
    m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

// updates the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update() {
    // add the speed variable to the current frame
    m_currentFrame++;

    // TODO: 1) Calculate the correct frame of animation to play based on currentFrame and speed
    // 2) set the texture rectangle properly (see constructor for sample)

}

const Vec2 &Animation::getSize() const {
    return m_size;
}

const std::string &Animation::getName() const {
    return m_name;
}

sf::Sprite &Animation::getSprite() {
    return m_sprite;
}


bool Animation::hasEnded() const {
    // TODO: detect when animation has ended (last frame was played) and return true
    return false;
}

