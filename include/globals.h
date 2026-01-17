#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"

// Game Objects
extern Snake snake1, snake2;
extern Food foods[MAX_FOODS];
extern Bomb bombs[MAX_BOMBS];
extern PowerUp powerups[MAX_POWERUPS];
extern Obstacle obstacles[MAX_OBSTACLES];

// Game State
extern GameMode mode;
extern GameState state;
extern GameState previousState;

// Counters and Timers
extern int level;
extern float gameTimer;
extern float difficultyTimer;
extern float bombTimer;
extern float powerupTimer;
extern float obstacleTimer;

// Settings and UI
extern HighScore highScores[MAX_HIGH_SCORES];
extern int selectedOption;
extern bool showGrid;
extern bool soundEnabled;
extern bool musicEnabled;
extern bool nightMode;
extern char playerName[20];

// Game Configuration state
extern int foodCount;
extern int bombCount;
extern int obstacleCount;
extern int mazeType;

// Sound effects
extern Sound eatSound;
extern Sound crashSound;
extern Sound powerupSound;
extern Sound bombSound;
extern Sound levelSound;
extern Music gameMusic;

// Textures
extern Texture2D backgroundTexture;
extern Texture2D menuBackgroundTexture;
extern Texture2D obstacleTexture;
extern Texture2D powerupTexture;

// Animation
extern Rectangle frameRec;
extern Rectangle tailFrameRec;
extern int currentFrame;
extern int tailCurrentFrame;
extern int framesCounter;
extern int framesSpeed;
extern int tailSpeed;

#endif
