#include "Player.h"

void Player::unitVector(sf::Vector2f &t_vector, float dt)
{
    float length = sqrt((t_vector.x * t_vector.x) + (t_vector.y * t_vector.y));
    t_vector = (t_vector * m_speed * dt) / length;
}

Player::Player() :
        m_runningAnim(m_sprite),
        m_walkingAnim(m_sprite),
        m_crouchingAnim(m_sprite),
        m_idlingAnim(m_sprite),
        m_startThrowingAnim(m_sprite, false),
        m_endThrowingAnim(m_sprite, false)
{
    m_speed = WALKING_SPEED;
    Player::loadTexture();
    // Get spritesheet data

    // Get sprite sheet data
    std::ifstream spriteSheetData("ASSETS/IMAGES/sprite_sheets/data/characters_sprite_sheet.json");
    nlohmann::json json;
    spriteSheetData >> json;

    for (auto &val: json["frames"])
    {
        std::string filename = val["filename"];

        std::string::size_type idleFound = filename.find("player/idling/");
        std::string::size_type runningFound = filename.find("player/running/");
        std::string::size_type startThrowingFound = filename.find("player/throwing/start/");
        std::string::size_type endThrowingFound = filename.find("player/throwing/end/");
        // Found something
        if (idleFound != std::string::npos)
        {
            nlohmann::json frame = val["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_idlingAnim.addFrame({sf::IntRect(x, y, width, height)});
        } else if (runningFound != std::string::npos)
        {
            nlohmann::json frame = val["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_runningAnim.addFrame({sf::IntRect(x, y, width, height), 0.025});
            m_walkingAnim.addFrame({sf::IntRect(x, y, width, height), 0.05});
            m_crouchingAnim.addFrame({sf::IntRect(x, y, width, height), 0.25});
        } else if (startThrowingFound != std::string::npos)
        {
            nlohmann::json frame = val["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_startThrowingAnim.addFrame({sf::IntRect(x, y, width, height)});
        } else if (endThrowingFound != std::string::npos)
        {
            nlohmann::json frame = val["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_endThrowingAnim.addFrame({sf::IntRect(x, y, width, height)});
        }
    }

    m_sprite.setPosition(100, 100);
    m_sprite.setOrigin(30, 26);

    m_powerBar.setSize(m_powerBarSize);
    m_powerBar.setFillColor(sf::Color::White);
    m_powerthrow.setSize(m_powerSize);
    m_powerthrow.setOutlineThickness(1);
    m_powerthrow.setFillColor(sf::Color::Red);

    m_staminaBar.setSize(m_staminaBarSize);
    m_staminaBar.setFillColor(sf::Color::Black);

    if (!m_bottleTexture.loadFromFile("ASSETS\\IMAGES\\Bottle.png"))
    {
        // simple error message if previous call fails
        std::cout << "problem loading backpack texture" << std::endl;
    }

    m_bottleSprite.setTexture(m_bottleTexture);
    m_bottleSprite.setScale(0.025, 0.025);
}


void Player::setDirection(int t_direction)
{
    switch (t_direction)
    {
        case NORTH:
            m_sprite.setRotation(270);
            break;
        case SOUTH:
            m_sprite.setRotation(90);
            break;
        case EAST:
            m_sprite.setRotation(0);
            break;
        case WEST:
            m_sprite.setRotation(180);
            break;
        case NORTHWEST:
            m_sprite.setRotation(225);
            break;
        case NORTHEAST:
            m_sprite.setRotation(315);
            break;
        case SOUTHWEST:
            m_sprite.setRotation(135);
            break;
        case SOUTHEAST:
            m_sprite.setRotation(45);
            break;
    }
}

void Player::update(sf::Time deltaTime ,sf::Vector2f t_position )
{
    move(deltaTime.asSeconds());
    if (m_velocity.x == 0 && m_velocity.y == 0)
        m_playerState = PlayerMovingState::IDLE;

    sf::Time elapsed = clock.getElapsedTime();

    // Update animations
    switch (m_playerState)
    {
        case PlayerMovingState::RUNNING:
            m_runningAnim.update(deltaTime.asSeconds());

            if (elapsed.asSeconds() >= 0.25)
            {
                Utils::playRandomSound(footstepRunSounds);
                clock.restart();
            }
            break;
        case PlayerMovingState::WALKING:
            m_walkingAnim.update(deltaTime.asSeconds());

            if (elapsed.asSeconds() >= 0.5)
            {
                Utils::playRandomSound(footstepWalkSounds);
                clock.restart();
            }

            break;
        case PlayerMovingState::CROUCHING:
            m_crouchingAnim.update(deltaTime.asSeconds());

            if (elapsed.asSeconds() >= 2.5)
            {
                Utils::playRandomSound(footstepSneakSounds);
                clock.restart();
            }
            break;
        case PlayerMovingState::IDLE:
            m_idlingAnim.update(deltaTime.asSeconds());
            break;
        default:
            m_idlingAnim.update(deltaTime.asSeconds());
    }
    boundryCheck();

    if (m_powerBarVisible)
    {
        m_endThrowingAnim.update(deltaTime.asSeconds());
        m_powerBar.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y);
        m_powerBar.setRotation(m_sprite.getRotation());

        if (m_powerSize.x < m_powerBarSize.x)
        {
            m_powerSize.x++;
        }

        m_powerthrow.setSize(m_powerSize);
        m_powerthrow.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y);
        m_powerthrow.setRotation(m_sprite.getRotation());

    } 

    
    m_staminaBarLvl.setSize(m_staminaBarLvlSize);
    m_staminaBar.setPosition(t_position.x, t_position.y + screen_Height / 4 - m_staminaBarSize.y-10);
    m_staminaBarLvl.setPosition(t_position.x+5, t_position.y + screen_Height / 4 - m_staminaBarSize.y - 10 + 2.5);
    if (m_playerState == PlayerMovingState::RUNNING && m_staminaBarLvlSize.x >=0)
    {
        m_staminaBarLvlSize.x--;
    }
    if (m_playerState != PlayerMovingState::RUNNING && m_staminaBarLvlSize.x <= 190)
    {
        m_staminaBarLvlSize.x = m_staminaBarLvlSize.x + 0.1f;
    }
    if (m_staminaBarLvlSize.x <= 2)
    {
        m_canRun = false;
        m_speed = WALKING_SPEED;
        m_playerState = PlayerMovingState::WALKING;
    }
    if ( m_staminaBarLvlSize.x >10)
    {
        m_canRun = true;
    }
    if (m_staminaBarLvlSize.x>=125)
    {
        m_staminaBarLvl.setFillColor(sf::Color::Green);
    }
    if (m_staminaBarLvlSize.x < 125 && m_staminaBarLvlSize.x >= 63)
    {
        m_staminaBarLvl.setFillColor(sf::Color::Yellow);
    }
    if (m_staminaBarLvlSize.x < 63)
    {
        m_staminaBarLvl.setFillColor(sf::Color::Red);
    }    
}


void Player::processEvents(sf::Event event)
{
    sf::Time elapsed = clock.getElapsedTime();

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::Space)
        {
            if (event.key.code == sf::Keyboard::LControl)
            {
                m_speed = CROUCHING_SPEED;
                m_playerState = PlayerMovingState::CROUCHING;
            }
            if (event.key.code == sf::Keyboard::Space && m_canRun==true)
            {
                m_speed = RUNNING_SPEED;
                m_playerState = PlayerMovingState::RUNNING;
            }
        }
    }

    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::P)
        {
            m_gameState = GameState::PAUSE;
        }

        if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::Space)
        {
            m_speed = WALKING_SPEED;
            m_playerState = PlayerMovingState::WALKING;
        }
        if (event.key.code == sf::Keyboard::RControl)
        {           
           m_powerBarVisible = false;

           

           if (m_readyToTHrow[0] == true)
           {
               m_readyToTHrow[0] = false;
               m_throw[0] = true;
               return;
           }
           if (m_readyToTHrow[1] == true)
           {
               m_readyToTHrow[1] = false;
               m_throw[1] = true;

           }
        
            //m_throw = false;
            m_endThrowingAnim.reset();
            m_powerSize.x = 0;
        }
    }
}

void Player::awayFrom(sf::Vector2f t_obstacle)
{
    sf::Vector2f gap = t_obstacle - m_sprite.getPosition();
    gap = (getRadius() / sqrt((gap.x * gap.x) + (gap.y * gap.y))) * gap;
    m_sprite.setPosition(t_obstacle - gap);
}

void Player::renderPowerBar(sf::RenderWindow &t_window)
{
    if (m_powerBarVisible)
    {     
        t_window.draw(m_powerBar);
        t_window.draw(m_powerthrow);
    }

    t_window.draw(m_staminaBar);
    t_window.draw(m_staminaBarLvl);

}

void Player::move(float dt)
{
    m_velocity = sf::Vector2f(0.0f, 0.0f);

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        && (m_playerState != PlayerMovingState::RUNNING && m_playerState != PlayerMovingState::CROUCHING))
        m_playerState = PlayerMovingState::WALKING;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    {
        if (m_readyToTHrow[0] == true)
        {
            m_powerBarVisible = true;
        }
        if (m_readyToTHrow[1] == true)
        {
            m_powerBarVisible = true;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        m_velocity.y += -m_speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        m_velocity.y += m_speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        m_velocity.x += -m_speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        m_velocity.x += m_speed * dt;

    if (m_velocity.y > 0 && m_velocity.x > 0)
    {
        setDirection(SOUTHEAST);
        unitVector(m_velocity, dt);
    } else if (m_velocity.y > 0 && m_velocity.x < 0)
    {
        setDirection(SOUTHWEST);
        unitVector(m_velocity, dt);
    } else if (m_velocity.y < 0 && m_velocity.x > 0)
    {
        setDirection(NORTHEAST);
        unitVector(m_velocity, dt);
    } else if (m_velocity.y < 0 && m_velocity.x < 0)
    {
        setDirection(NORTHWEST);
        unitVector(m_velocity, dt);
    } else if (m_velocity.y > 0)
        setDirection(SOUTH);
    else if (m_velocity.y < 0)
        setDirection(NORTH);
    else if (m_velocity.x > 0)
        setDirection(EAST);
    else if (m_velocity.x < 0)
        setDirection(WEST);

    m_sprite.move(m_velocity);
}

sf::Vector2f Player::getVelocity()
{
    return m_velocity;
}

sf::Sprite Player::getSprite()
{
    return m_sprite;
}

float Player::m_throwPower()
{
    float t = m_powerBarSize.x;
    m_powerSize.x = 0;
    return t;
}

void Player::boundryCheck()
{
    if (m_sprite.getPosition().x > screen_Width - m_sprite.getTextureRect().width)
    {
        m_sprite.setPosition(screen_Width - m_sprite.getTextureRect().width, m_sprite.getPosition().y);
    }
    if (m_sprite.getPosition().x < getRadius())
    {
        m_sprite.setPosition(getRadius(), m_sprite.getPosition().y);
    }
    if (m_sprite.getPosition().y > screen_Height - m_sprite.getTextureRect().height)
    {
        m_sprite.setPosition(m_sprite.getPosition().x, screen_Height - m_sprite.getTextureRect().height);
    }
    if (m_sprite.getPosition().y < getRadius())
    {
        m_sprite.setPosition(m_sprite.getPosition().x, getRadius());
    }
}

void Player::loadSoundHolder(SoundHolder &soundHolder)
{
    footstepWalkSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Walk_Sand1)));
    footstepWalkSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Walk_Sand2)));
    footstepWalkSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Walk_Sand3)));
    footstepWalkSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Walk_Sand4)));
    footstepWalkSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Walk_Sand5)));
    footstepWalkSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Walk_Sand6)));
    footstepWalkSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Walk_Sand7)));
    footstepWalkSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Walk_Sand8)));

    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand1)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand2)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand3)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand4)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand5)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand6)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand7)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand8)));

    footstepSneakSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Sneak_Sand1)));
    footstepSneakSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Sneak_Sand2)));
    footstepSneakSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Sneak_Sand3)));
    footstepSneakSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Sneak_Sand4)));

    for (auto sound: footstepWalkSounds)
        sound->setVolume(20);

    for (auto sound: footstepRunSounds)
        sound->setVolume(20);

    for (auto sound: footstepSneakSounds)
        sound->setVolume(20);
}



