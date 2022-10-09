#include "SoundHolder.h"

void SoundHolder::load(Sounds id, const std::string &filename)
{
    std::unique_ptr<sf::SoundBuffer> sound(new sf::SoundBuffer);
    if (!sound->loadFromFile(filename))
        throw std::runtime_error("SoundHolder::load - Failed to load " + filename);

    auto inserted = m_soundMap.insert(std::make_pair(id, std::move(sound)));
    assert(inserted.second);
}

template<typename Parameter>
void SoundHolder::load(Sounds id, const std::string &filename, const Parameter &secondParam)
{
    std::unique_ptr<sf::SoundBuffer> sound(new sf::SoundBuffer);
    if (!sound->loadFromFile(filename, secondParam))
        throw std::runtime_error("SoundHolder::load - Failed to load " + filename);

    auto inserted = m_soundMap.insert(std::make_pair(id, std::move(sound)));
    assert(inserted.second);
}

sf::SoundBuffer &SoundHolder::get(Sounds id)
{
    auto found = m_soundMap.find(id);
    assert(found != m_soundMap.end());

    return *found->second;
}

const sf::SoundBuffer &SoundHolder::get(Sounds id) const
{
    auto found = m_soundMap.find(id);
    return *found->second;
}
