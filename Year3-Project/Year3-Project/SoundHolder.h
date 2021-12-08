#ifndef YEAR3_PROJECT_SOUNDHOLDER_H
#define YEAR3_PROJECT_SOUNDHOLDER_H

#include <string>
#include <map>
#include <cassert>

#include <SFML/Audio.hpp>

#include "SoundIdentifiers.h"

class SoundHolder
{
public:
    void load(Sounds id, const std::string& filename);

    template<typename Parameter>
    void load(Sounds id, const std::string& filename, const Parameter& secondParam);

    sf::SoundBuffer& get(Sounds id);
    const sf::SoundBuffer& get(Sounds id) const;

private:
    std::map<Sounds, std::unique_ptr<sf::SoundBuffer>> m_soundMap;
};


#endif //YEAR3_PROJECT_SOUNDHOLDER_H
