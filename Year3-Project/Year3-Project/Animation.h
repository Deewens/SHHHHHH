#ifndef YEAR3_PROJECT_ANIMATION_H
#define YEAR3_PROJECT_ANIMATION_H

#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation();

    void addFrame(sf::IntRect rect);
    void setSpriteSheet(const sf::Texture& texture);
    const sf::Texture* getSpriteSheet() const;
    std::size_t getSize() const;
    const sf::IntRect& getFrame(std::size_t n) const;

private:
    std::vector<sf::IntRect> m_frames;
    const sf::Texture* m_textures;
};


#endif //YEAR3_PROJECT_ANIMATION_H
