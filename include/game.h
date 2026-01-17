#ifndef GAME_H
#define GAME_H

#include "types.h"
#include "globals.h"
#include "utils.h"
#include "entities.h"
#include "ui.h"

void handleInput();
void checkCollisions();
void updateGameplay(float deltaTime);
void drawGameplay();

#endif
