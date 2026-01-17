#ifndef ENTITIES_H
#define ENTITIES_H

#include "types.h"
#include "globals.h"
#include "utils.h"

void initGameObjects();
void generateNewFood();
void generateNewBomb();
void generateNewPowerup();
void generateObstacles();
void updateSnake(Snake *snake, float deltaTime);
void updateComputerSnake();

void drawSnake(Snake snake);
void drawFoods();
void drawBombs();
void drawPowerups();
void drawObstacles();

#endif
