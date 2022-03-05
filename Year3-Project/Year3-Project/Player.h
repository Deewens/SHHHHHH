#pragma once

#include <iostream>
#include <fstream>
#include <random>

#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "Character.h"
#include "Animation.h"
#include "json.hpp"
#include <math.h>
#include "SoundHolder.h"
#include "Utils.h"
#include "ParticleSystem.h"



struct Direction {
    bool up = false;
    bool down = false;
    bool right = false;
    bool left = false;
};

class Player : public Character
{

private:
    bool m_isAlive = true;

    bool m_pausePressed = false;

    Animation m_crouchingAnim;
    Animation m_walkingAnim;
    Animation m_runningAnim;

    Animation m_idlingAnim;
    Animation m_startThrowingAnim;
    Animation m_endThrowingAnim;

    Direction m_movementDir;
    sf::Vector2f m_velocity;

    void unitVector(sf::Vector2f& t_vector, float dt);
    void setDirection(int t_direction);
    void boundryCheck();
    sf::Vector2f getVelocity();



    bool m_powerBarVisible = false;



    std::vector<sf::Sound*> footstepWalkSounds;
    std::vector<sf::Sound*> footstepRunSounds;
    std::vector<sf::Sound*> footstepSneakSounds;
    sf::Clock clock;

    sf::RectangleShape m_staminaBar;
    sf::Vector2f m_staminaBarSize{ 250,35 };

    sf::RectangleShape m_staminaBarLvl;
    sf::Vector2f m_staminaBarLvlSize{ 240,30 };

    bool m_canRun = true;    

    sf::Texture m_powerTexture;
    sf::Sprite m_powerSprite;

    sf::Vector2f powerSpriteScale{0.1,0.1};

    NoiseLevels m_noiseLevel = NoiseLevels::WHITE;

    void bottleMovement();

    float m_bottleRotate = 0;

    float m_bottleSpeed = 10.0f;

    float m_bottleAirTime[2] = {1,1 };

    sf::Vector2f m_worldCenter{};

    float m_heading{};

    ParticleSystem m_Impact[2];


    sf::Color m_color = sf::Color(210, 105, 30);


public:

    Player(sf::Texture& t_texture);
 
    void loadSoundHolder(SoundHolder& soundHolder);

    void update(sf::Time deltaTime , sf::Vector2f t_position) ;

    void processEvents(sf::Event event);

    void awayFrom(sf::Vector2f t_obstacle);

    void renderPowerBar(sf::RenderWindow& t_window);

    sf::Sprite getSprite();
    
    bool m_readyToTHrow[2] = { false,false };

    bool m_throw[2] = { false,false };

    void move(float dt);

    sf::Texture m_bottleTexture;
    sf::Sprite m_bottleSprite[2];

    float bottleSpriteRadius();

    float m_spalshTime[2] = { 80,80 };

    bool m_bottleBreak[2] = { false,false };

    sf::Vector2f m_newPos{};

    sf::Vector2f m_offSet{};

    void setupNewPlayer(int t_gridIndex, int t_rotation);

    NoiseLevels getNoiseLevel();

    sf::Vector2f bottleLocation();
};

