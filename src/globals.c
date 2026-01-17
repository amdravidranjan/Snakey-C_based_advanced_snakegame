#include "globals.h"

// Global variables definition
Snake snake1, snake2;
Food foods[MAX_FOODS];
Bomb bombs[MAX_BOMBS];
PowerUp powerups[MAX_POWERUPS];
Obstacle obstacles[MAX_OBSTACLES];

GameMode mode = CLASSIC;
GameState state = MAIN_MENU;
GameState previousState = MAIN_MENU;

int level = 1;
float gameTimer = 0.0f;
float difficultyTimer = 0.0f;
float bombTimer = 0.0f;
float powerupTimer = 0.0f;
float obstacleTimer = 0.0f;

HighScore highScores[MAX_HIGH_SCORES];
int selectedOption = 0;
bool showGrid = false;
bool soundEnabled = true;
bool musicEnabled = true;
bool nightMode = false;
char playerName[20] = "Player";

int foodCount = 1; 
int bombCount = 0; 
int obstacleCount = 0; 
int mazeType = 0; 

// Sound effects
Sound eatSound;
Sound crashSound;
Sound powerupSound;
Sound bombSound;
Sound levelSound;
Music gameMusic;

// Textures
Texture2D backgroundTexture;
Texture2D menuBackgroundTexture;
Texture2D obstacleTexture;
Texture2D powerupTexture;

Rectangle frameRec;
Rectangle tailFrameRec;
int currentFrame = 0;
int tailCurrentFrame = 0;
int framesCounter = 0;
int framesSpeed = 8;
int tailSpeed = 10;
