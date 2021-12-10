#ifndef YEAR3_PROJECT_UTILS_H
#define YEAR3_PROJECT_UTILS_H

#include <random>

#include <SFML/Audio.hpp>

#include <cmath>

class Utils
{
public:
    static int generateRandomNumber(int from, int to);
    static void playRandomSound(std::vector<sf::Sound*>& sounds);

    static float getDistanceBetweenPoints(sf::Vector2f p, sf::Vector2f q);

private:
    Utils(); // This class is not an object and cannot be instantiated - Not Implemented
    Utils(const Utils& c); // Copy Constructor - Not Implemented
    Utils& operator=(const Utils&c); // Assignment Operator - Not Implemented
};


#endif //YEAR3_PROJECT_UTILS_H
