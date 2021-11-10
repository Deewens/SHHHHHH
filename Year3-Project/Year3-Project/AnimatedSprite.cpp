#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(sf::Time frameTime, bool paused, bool looped) :
    m_animation(nullptr),
    m_frameTime(frameTime),
    m_currentFrame(0),
    m_isPaused(paused),
    m_isLooped(looped),
    m_texture(nullptr)
{
}

void AnimatedSprite::update(sf::Time deltaTime)
{
    // If not paused and have a valid animation
    if (!m_isPaused && m_animation)
    {
        // Add delta time
        m_currentTime += deltaTime;

        // If current time is bigger than the frame time, advance one frame
        if (m_currentTime >= m_frameTime)
        {
            // Reset time but keep the remainder
            m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

            // Get next frame index
            if (m_currentFrame + 1 < m_animation->getSize()) // Check if we are still in the bound of the std::vector
                m_currentFrame++;
            else
            {
                // Animation has ended (reached the last sprite)
                if (!m_isLooped)
                    m_isPaused = true;
                else
                    m_currentFrame = 0; // Reset to start
            }
        }

        // set the current frame, not reseting the time
        setFrame(m_currentFrame, false);
    }
}

/// Set the animation to be used by the AnimatedSprite
/// \param animation animation used by the AnimatedSprite
void AnimatedSprite::setAnimation(const Animation &animation)
{
    m_animation = &animation;
    m_texture = m_animation->getSpriteSheet();
    m_currentFrame = 0;
    setFrame(m_currentFrame);
}

void AnimatedSprite::setFrameTime(sf::Time time)
{
    m_frameTime = time;
}

void AnimatedSprite::play()
{
    m_isPaused = false;
}

/// Change the animation with the one given and play it directly
/// \param animation the animation to be used
void AnimatedSprite::play(const Animation &animation)
{
    if (getAnimation() != &animation)
        setAnimation(animation);

    play();
}

void AnimatedSprite::pause()
{
    m_isPaused = true;
}

void AnimatedSprite::stop()
{
    m_isPaused = true;
    m_currentFrame = 0;
    setFrame(m_currentFrame);
}

void AnimatedSprite::setLooped(bool looped)
{
    m_isLooped = looped;
}

void AnimatedSprite::setColor(const sf::Color& color)
{
    // Update the vertices color
    m_vertices[0].color = color;
    m_vertices[1].color = color;
    m_vertices[2].color = color;
    m_vertices[3].color = color;
}

const Animation *AnimatedSprite::getAnimation() const
{
    return m_animation;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
    sf::IntRect rect = m_animation->getFrame(m_currentFrame);

    auto width = static_cast<float>(std::abs(rect.width));
    auto height = static_cast<float>(std::abs(rect.height));

    return {0.f, 0.f, width, height};
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

bool AnimatedSprite::isLooped()
{
    return !m_isLooped;
}

bool AnimatedSprite::isPlaying()
{
    return !m_isPaused;
}

sf::Time AnimatedSprite::getFrameTime() const
{
    return m_frameTime;
}

void AnimatedSprite::setFrame(std::size_t newFrame, bool resetTime)
{
    if (m_animation)
    {
        sf::IntRect rect = m_animation->getFrame(newFrame);

        m_vertices[0].position = sf::Vector2f(0.f, 0.f);
        m_vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
        m_vertices[2].position = sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
        m_vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

        float left = static_cast<float>(rect.left) + 0.0001f;
        float right = left + static_cast<float>(rect.width);
        float top = static_cast<float>(rect.top);
        float bottom = top + static_cast<float>(rect.height);

        m_vertices[0].texCoords = sf::Vector2f(left, top);
        m_vertices[1].texCoords = sf::Vector2f(left, bottom);
        m_vertices[2].texCoords = sf::Vector2f(right, bottom);
        m_vertices[3].texCoords = sf::Vector2f(right, top);
    }

    if (resetTime)
        m_currentTime = sf::Time::Zero;
}

void AnimatedSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (m_animation && m_texture)
    {
        states.transform *= getTransform();
        states.texture = m_texture;
        target.draw(m_vertices, 4, sf::Quads, states);
    }
}
