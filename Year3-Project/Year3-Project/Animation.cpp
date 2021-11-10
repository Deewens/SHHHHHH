#include "Animation.h"

Animation::Animation() : m_textures(nullptr)
{
}

void Animation::addFrame(sf::IntRect rect)
{
    m_frames.push_back(rect);
}

void Animation::setSpriteSheet(const sf::Texture &texture)
{
    m_textures = &texture;
}

const sf::Texture* Animation::getSpriteSheet() const
{
    return m_textures;
}

std::size_t Animation::getSize() const
{
    return m_frames.size();
}

const sf::IntRect &Animation::getFrame(std::size_t n) const
{
    return m_frames[n];
}

