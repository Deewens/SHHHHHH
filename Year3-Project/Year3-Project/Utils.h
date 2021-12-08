#ifndef YEAR3_PROJECT_UTILS_H
#define YEAR3_PROJECT_UTILS_H

#include <random>

#include <SFML/Audio.hpp>

class Utils
{
public:
    static int generateRandomNumber(int from, int to);
    static void playRandomSound(std::vector<sf::Sound*>& sounds);

private:
    Utils(); // This class is not an object and cannot be instantiated - Not Implemented
    Utils(const Utils& c); // Copy Constructor - Not Implemented
    Utils& operator=(const Utils&c); // Assignment Operator - Not Implemented
};


#endif //YEAR3_PROJECT_UTILS_H
