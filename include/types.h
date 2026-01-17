#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 1200
#define HEIGHT 680
#define TILE_SIZE 20
#define SCALED_TILE_SIZE 40
#define MAX_SNAKE_LENGTH 500
#define MAX_FOODS 5
#define MAX_BOMBS 3
#define MAX_POWERUPS 2
#define MAX_OBSTACLES 15
#define MAX_HIGH_SCORES 5
#define INITIAL_SNAKE_LENGTH 5
#define INITIAL_SNAKE_SPEED 1

typedef enum { CLASSIC, MULTIPLAYER, COMPUTER, SURVIVAL, MAZE } GameMode;
typedef enum { MAIN_MENU, GAMEPLAY, GAME_OVER, PAUSE, HIGH_SCORES, CONTROLS, SETTINGS } GameState;
typedef enum { SPEED, INVINCIBLE, DOUBLE_POINTS, GHOST } PowerUpType;

typedef struct {
    Vector2 position[MAX_SNAKE_LENGTH],recdir[MAX_SNAKE_LENGTH];
    int length;
    Vector2 direction;
    Vector2 previousDir;
    float moveTimer;
    float moveInterval;
    float baseInterval; // Store the original speed
    bool reversing;
    int pivotPoint;
    Color color;
    Texture2D texture;
    Texture2D headTexture;
    Texture2D tailTexture;
    bool isAlive;
    int score;
    bool isGhost;        // Can pass through walls and obstacles
    bool isInvincible;   // Cannot die from collisions
    float powerupTimer;  // Timer for active powerups
    float flashTimer;    // Visual indicator for powerup active
    bool isPowerupActive;
} Snake;

typedef struct {
    Vector2 position;
    Texture2D texture;
    bool active;
    int points;
    Color color;
} Food;

typedef struct {
    Vector2 position;
    Texture2D texture;
    bool active;
} Bomb;

typedef struct {
    Vector2 position;
    Texture2D texture;
    bool active;
    PowerUpType type;
    float duration;
    Color color;
} PowerUp;

typedef struct {
    Vector2 position;
    Texture2D texture;
    Rectangle bounds;
} Obstacle;

typedef struct {
    char name[20];
    int score;
    GameMode mode;
} HighScore;

#endif
