#include "utils.h"

// Function to check if a position is inside any obstacle
bool isInsideObstacle(Vector2 position) {
    for (int i = 0; i < obstacleCount; i++) {
        if (CheckCollisionPointRec(position, obstacles[i].bounds)) {
            return true;
        }
    }
    return false;
}

float vector2distance(Vector2 v1, Vector2 v2) {
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
}

// Function to check if a position is inside any snake
bool isInsideSnake(Vector2 position) {
    for (int i = 0; i < snake1.length; i++) {
        if (vector2distance(position, snake1.position[i]) < SCALED_TILE_SIZE) {
            return true;
        }
    }
    
    if (mode == MULTIPLAYER || mode == COMPUTER) {
        for (int i = 0; i < snake2.length; i++) {
            if (vector2distance(position, snake2.position[i]) < SCALED_TILE_SIZE) {
                return true;
            }
        }
    }
    
    return false;
}

// Function to generate a position that doesn't collide with any obstacle or snake
Vector2 generateSafePosition() {
    Vector2 position;
    bool validPosition = false;
    
    // Try to find a valid position
    int attempts = 0;
    while (!validPosition && attempts < 100) {
        position.x = 40 + (rand() % ((WIDTH - 120) / SCALED_TILE_SIZE)) * SCALED_TILE_SIZE;
        position.y = 40 + (rand() % ((HEIGHT - 120) / SCALED_TILE_SIZE)) * SCALED_TILE_SIZE;
        
        validPosition = !isInsideObstacle(position) && !isInsideSnake(position);
        attempts++;
    }
    
    // If no valid position found after 100 attempts, just return a position in the center
    if (!validPosition) {
        position.x = WIDTH / 2;
        position.y = HEIGHT / 2;
    }
    
    return position;
}

// Function to check if two directions are opposite
bool isOppositeDirection(Vector2 dir1, Vector2 dir2) {
    return (dir1.x == -dir2.x && dir1.y == -dir2.y) ||
           (dir1.y == -dir2.y && dir1.x == -dir2.x);
}

// Function to get angle for snake head rotation
float getHeadRotation(Vector2 direction) {
    if (direction.x == 1) return 0.0f;
    if (direction.x == -1) return 180.0f;
    if (direction.y == 1) return 90.0f;
    if (direction.y == -1) return 270.0f;
    return 0.0f;
}

bool vector2equals(Vector2 v1, Vector2 v2) {
    return (v1.x == v2.x) && (v1.y == v2.y);
}
