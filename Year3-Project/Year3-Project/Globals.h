#pragma once

const int screen_Width = 1440;
const int screem_Height = 900;

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

enum class GameState { MENU, GAMEPLAY, EXIT, OPTIONS};


static GameState m_gameState = GameState::MENU;

enum class PlayeMovingState {IDLE,CROUCHING , WALKING , RUNNING};

static PlayeMovingState m_playerState = PlayeMovingState::IDLE;

