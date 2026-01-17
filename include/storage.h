#ifndef STORAGE_H
#define STORAGE_H

#include "types.h"
#include "globals.h"

void saveHighScores(void);
void loadHighScores(void);
void addHighScore(int score, GameMode scoreMode);

#endif
