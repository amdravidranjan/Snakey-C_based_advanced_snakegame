#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include "globals.h"

bool isInsideObstacle(Vector2 position);
float vector2distance(Vector2 v1, Vector2 v2);
bool isInsideSnake(Vector2 position);
Vector2 generateSafePosition();
bool isOppositeDirection(Vector2 dir1, Vector2 dir2);
float getHeadRotation(Vector2 direction);
bool vector2equals(Vector2 v1, Vector2 v2);

#endif
