#ifndef YEAR3_PROJECT_UTILS_H
#define YEAR3_PROJECT_UTILS_H

#include <random>
#include <cmath>
#include <map>

#include <SFML/Audio.hpp>

#include "Environment.h"
#include "Graph.h"

class Utils
{
public:
    static int generateRandomNumber(int from, int to);

    static void playRandomSound(std::vector<sf::Sound *> &sounds);

    static float getDistanceBetweenPoints(sf::Vector2f p, sf::Vector2f q);

    template <typename T>
    static float magnitude(sf::Vector2<T> v);

    template<typename T>
    static sf::Vector2<T> normalize(sf::Vector2<T> a);

private:
    Utils(); // This class is not an object and cannot be instantiated - Not Implemented
    Utils(const Utils &c); // Copy Constructor - Not Implemented
    Utils &operator=(const Utils &c); // Assignment Operator - Not Implemented
};


#endif //YEAR3_PROJECT_UTILS_H
