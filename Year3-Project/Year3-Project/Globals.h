#pragma once

const int screen_Width = 1200;
const int screen_Height = 900;

static const int tileSize{ 60 };

const int m_gridCols = screen_Width / tileSize;
const int m_gridRows = screen_Height / tileSize;


//The direction the sprite is moving in
const int static WEST = 1;
const int static EAST = 2;
const int static NORTH = 3;
const int static SOUTH = 4;
const int static NORTHEAST = 5;
const int static NORTHWEST = 6;
const int static SOUTHEAST = 7;
const int static SOUTHWEST = 8;

enum class Directions {NORTH,EAST,WEST,SOUTH,NORTHEAST,NORTHWEST,SOUTHEAST,SOUTHWEST};

static Directions m_direction = Directions::EAST;

//menu option number

const int MENU_OPTION_NUMBER = 4;

//menu button states

enum class ButtonState { READY, OVER, DOWN };

enum class GameState { MENU, GAMEPLAY, EXIT, HELP,PAUSE,LVLBUILDER, WIN, LOSE};

extern GameState m_gameState;

enum class PlayerMovingState {IDLE,CROUCHING , WALKING , RUNNING};

extern PlayerMovingState m_playerState;

const float CROUCHING_SPEED = 25.f;
const float WALKING_SPEED = 75.f;
const float RUNNING_SPEED = 150.f;

enum class NoiseLevels {WHITE = 0, GREEN = 1, YELLOW = 2, RED = 3};

const float PI = 3.14159265359;

static int const  MAX_PARTICLES = 50;

extern int level;

