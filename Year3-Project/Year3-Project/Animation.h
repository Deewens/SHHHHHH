#ifndef YEAR3_PROJECT_ANIMATION_H
#define YEAR3_PROJECT_ANIMATION_H

#include <SFML/Graphics.hpp>

struct Frame
{
    sf::IntRect rect;
    double duration = 0.2; // In seconds
};

class Animation
{
private:
    std::vector<Frame> frames;
    double totalLength;
    double progress;
    sf::Sprite &target;
    bool isLooping;

public:
    Animation(sf::Sprite &target, bool looping = true);
    virtual ~Animation();
    void addFrame(Frame&& frame);
    void update(double deltaTime);
    void reset();
    const double getLength() const { return totalLength; }
};


#endif //YEAR3_PROJECT_ANIMATION_H
