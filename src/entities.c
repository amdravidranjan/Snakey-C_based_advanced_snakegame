#include "entities.h"

// Function to generate a new food
void generateNewFood() {
    for (int i = 0; i < MAX_FOODS; i++) {
        if (!foods[i].active) {
            foods[i].position = generateSafePosition();
            foods[i].active = true;
            foods[i].points = (rand() % 3) + 1; // 1-3 points per food
            
            // Set color based on points
            switch (foods[i].points) {
                case 1: foods[i].color = RED; break;
                case 2: foods[i].color = GOLD; break;
                case 3: foods[i].color = PURPLE; break;
            }
            
            break;
        }
    }
}

// Function to generate a new bomb
void generateNewBomb() {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (!bombs[i].active) {
            bombs[i].position = generateSafePosition();
            bombs[i].active = true;
            break;
        }
    }
}

// Function to generate a new powerup
void generateNewPowerup() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerups[i].active) {
            powerups[i].position = generateSafePosition();
            powerups[i].active = true;
            powerups[i].type = (PowerUpType)(rand() % 4); // Random powerup type
            powerups[i].duration = 10.0f; // 10 seconds duration
            
            // Set color based on powerup type
            switch (powerups[i].type) {
                case SPEED: powerups[i].color = BLUE; break;
                case INVINCIBLE: powerups[i].color = GREEN; break;
                case DOUBLE_POINTS: powerups[i].color = GOLD; break;
                case GHOST: powerups[i].color = GRAY; break;
            }
            
            break;
        }
    }
}

// Function to generate obstacles for the level
void generateObstacles() {
    // Clear existing obstacles
    obstacleCount = 0;
    
    // Different obstacle patterns based on level and maze type
    if (mode == MAZE) {
        // Generate maze-like obstacles
        int pattern = rand() % 3; // Randomize pattern
        
        switch (pattern) {
            case 0: // Simple horizontal barriers
                for (int i = 0; i < 3; i++) {
                    // Align X: 280 (7*40)
                    obstacles[obstacleCount].position.x = 280; 
                    // Align Y: 160, 320, 480
                    obstacles[obstacleCount].position.y = 160 + i * 160;
                    
                    obstacles[obstacleCount].bounds = (Rectangle){
                        obstacles[obstacleCount].position.x,
                        obstacles[obstacleCount].position.y,
                        600, // 15*40
                        SCALED_TILE_SIZE
                    };
                    obstacleCount++;
                }
                break;
                
            case 1: // Zigzag pattern
                // Use fixed logic to ensure grid alignment and screen bounds
                for (int i = 0; i < 3; i++) {
                    float width = 720; // 18 * 40
                    
                    if (i % 2 == 0) {
                        // Left aligned
                        obstacles[obstacleCount].position.x = 80; // 2 * 40
                    } else {
                        // Right aligned
                        obstacles[obstacleCount].position.x = WIDTH - 80 - width; // 1200 - 80 - 720 = 400
                    }
                    
                    obstacles[obstacleCount].position.y = 120 + i * 160; // 3*40 + i*4*40
                    
                    obstacles[obstacleCount].bounds = (Rectangle){
                        obstacles[obstacleCount].position.x,
                        obstacles[obstacleCount].position.y,
                        width,
                        SCALED_TILE_SIZE
                    };
                    obstacleCount++;
                }
                break;
                
            case 2: // Rooms pattern
                // Vertical walls
                for (int i = 0; i < 2; i++) {
                    obstacles[obstacleCount].position.x = 400 * (i+1); // 400, 800 (Both divisible by 40)
                    obstacles[obstacleCount].position.y = 80; // 2 * 40
                    obstacles[obstacleCount].bounds = (Rectangle){
                        obstacles[obstacleCount].position.x,
                        obstacles[obstacleCount].position.y,
                        SCALED_TILE_SIZE,
                        520 // 13 * 40
                    };
                    obstacleCount++;
                }
                
                // Horizontal corridors
                for (int i = 0; i < 2; i++) {
                    obstacles[obstacleCount].position.x = 80; // 2 * 40
                    obstacles[obstacleCount].position.y = (int)((HEIGHT/3.0f * (i+1)) / SCALED_TILE_SIZE) * SCALED_TILE_SIZE; // Snap to grid
                    obstacles[obstacleCount].bounds = (Rectangle){
                        obstacles[obstacleCount].position.x,
                        obstacles[obstacleCount].position.y,
                        1040, // 1200 - 160
                        SCALED_TILE_SIZE
                    };
                    obstacleCount++;
                }
                break;
        }
    } else {
        // Regular obstacles based on level
        int numObstacles = level > 10 ? 10 : level;
        
        for (int i = 0; i < numObstacles && obstacleCount < MAX_OBSTACLES; i++) {
            Vector2 position = generateSafePosition();
            
            // Determine if horizontal or vertical obstacle
            bool isHorizontal = (rand() % 2 == 0);
            float width, height;
            
            if (isHorizontal) {
                width = (rand() % 5 + 3) * SCALED_TILE_SIZE;
                height = SCALED_TILE_SIZE;
            } else {
                width = SCALED_TILE_SIZE;
                height = (rand() % 5 + 3) * SCALED_TILE_SIZE;
            }
            
            obstacles[obstacleCount].position = position;
            obstacles[obstacleCount].bounds = (Rectangle){
                position.x,
                position.y,
                width,
                height
            };
            
            // Check if this obstacle would block the play area
            if (position.x + width < WIDTH - 40 && position.y + height < HEIGHT - 40) {
                obstacleCount++;
            }
        }
    }
    
    // Load texture for each obstacle
    for (int i = 0; i < obstacleCount; i++) {
        obstacles[i].texture = obstacleTexture;
    }
}

// Function to initialize game objects
void initGameObjects() {
    // Initialize snake1 (player 1)
    snake1.length = 5;
    snake1.direction = (Vector2){1, 0};
    for(int i=0;i<snake1.length;i++)
    {
        snake1.recdir[i]=(Vector2){1, 0};
    }
    snake1.previousDir = snake1.direction;
    snake1.moveTimer = 0.0f;
    snake1.moveInterval = 0.1f;
    snake1.baseInterval = 0.1f;
    snake1.reversing = false;
    snake1.pivotPoint = 0;
    snake1.color = GREEN;
    snake1.isAlive = true;
    snake1.score = 0;
    snake1.isGhost = false;
    snake1.isInvincible = false;
    snake1.powerupTimer = 0.0f;
    snake1.flashTimer = 0.0f;
    snake1.isPowerupActive = false;
    
    for (int i = 0; i < snake1.length; i++) {
        snake1.position[i] = (Vector2){WIDTH/2 - i*SCALED_TILE_SIZE, HEIGHT-160};
    }
    
    // Initialize snake2 (player 2 or computer)
    if (mode == MULTIPLAYER || mode == COMPUTER) {
        snake2.length = 5;
        snake2.direction = (Vector2){-1, 0};
        snake2.previousDir = snake2.direction;
        snake2.moveTimer = 0.0f;
        snake2.moveInterval = 0.1f;
        snake2.baseInterval = 0.1f;
        snake2.reversing = false;
        snake2.pivotPoint = 0;
        snake2.color = BLUE;
        snake2.isAlive = true;
        snake2.score = 0;
        snake2.isGhost = false;
        snake2.isInvincible = false;
        snake2.powerupTimer = 0.0f;
        snake2.flashTimer = 0.0f;
        snake2.isPowerupActive = false;
        
        for (int i = 0; i < snake2.length; i++) {
            snake2.position[i] = (Vector2){WIDTH/2 + 150 + i*SCALED_TILE_SIZE, HEIGHT/8};
        }
    }
    
    // Initialize foods, bombs, powerups
    for (int i = 0; i < MAX_FOODS; i++) {
        foods[i].active = false;
    }
    
    for (int i = 0; i < MAX_BOMBS; i++) {
        bombs[i].active = false;
    }
    
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerups[i].active = false;
    }
    
    // Reset timers
    gameTimer = 0.0f;
    difficultyTimer = 0.0f;
    bombTimer = 0.0f;
    powerupTimer = 0.0f;
    obstacleTimer = 0.0f;
    
    // Set initial game elements
    foodCount = 1; // Start with one food
    bombCount = mode == SURVIVAL ? 1 : 0; // Start with a bomb in survival mode
    obstacleCount = 0;
    
    // Generate obstacles first so food doesn't spawn on them
    if (mode == MAZE) {
        generateObstacles(); 
    }

    // Generate initial foods
    for (int i = 0; i < foodCount; i++) {
        generateNewFood();
    }
    
    // Generate initial bombs
    for (int i = 0; i < bombCount; i++) {
        generateNewBomb();
    }
    
    // Reset level
    level = 1;
}

// Function to update snake movement
void updateSnake(Snake *snake, float deltaTime) {
    if (!snake->isAlive) return;

    snake->moveTimer += deltaTime;

    // Update powerup timers if active
    if (snake->isPowerupActive) {
        snake->powerupTimer -= deltaTime;
        snake->flashTimer += deltaTime;

        if (snake->powerupTimer <= 0) {
            // Reset powerup effects
            snake->moveInterval = snake->baseInterval;
            snake->isGhost = false;
            snake->isInvincible = false;
            snake->isPowerupActive = false;
        }
    }

    // Reversal handling
    if (snake->reversing) {
        if (snake->moveTimer >= snake->moveInterval) {
            snake->moveTimer -= snake->moveInterval;

            // Move the head in the new direction
            Vector2 newHeadPos = snake->position[0];
            newHeadPos.x += snake->direction.x * SCALED_TILE_SIZE;
            newHeadPos.y += snake->direction.y * SCALED_TILE_SIZE;

            // Shift all positions to make room for new head
            for (int i = snake->length; i > 0; i--) {
                snake->position[i] = snake->position[i - 1];
            }
            snake->position[0] = newHeadPos;

            // **NEW: Check if head collided with body**
            bool collidedWithBody = false;
            for (int i = 1; i < snake->length; i++) {
                if (vector2equals(snake->position[0], snake->position[i])) {
                    collidedWithBody = true;
                    break;
                }
            }

            // Remove a segment from the tail during reversal **AND collision**
            if (collidedWithBody && snake->pivotPoint > 0) {
                snake->length--;
                if ((snake->score-2)>=0){snake->score=(snake->score)-2;}
                snake->pivotPoint--;

                if (snake->length <= 1) {
                    snake->isAlive = false;
                    snake->reversing = false;
                    if (soundEnabled) PlaySound(crashSound);
                }
            } else {
                // Reversal complete (or no collision)
                snake->reversing = false;
                snake->previousDir = snake->direction;
            }
        }
    } else if (snake->moveTimer >= snake->moveInterval) {
        snake->moveTimer -= snake->moveInterval;
        // dir =1 is horizontal dir = 2 is vertical
        int dir =1;
        // Normal movement
        snake->recdir[0]=snake->direction;
        for (int i = snake->length - 1; i > 0; i--) {
            snake->position[i] = snake->position[i - 1];
           if(i!=snake->length-1)
           snake->recdir[i] = snake->recdir[i-1];

        }

        snake->position[0].x += snake->direction.x * SCALED_TILE_SIZE;
        snake->position[0].y += snake->direction.y * SCALED_TILE_SIZE;

        // Handle wrapping around the screen for ghost mode
        if (snake->isGhost) {
            if (snake->position[0].x < 40) snake->position[0].x = WIDTH - 80;
            if (snake->position[0].x >= WIDTH - 40) snake->position[0].x = 40;
            if (snake->position[0].y < 40) snake->position[0].y = HEIGHT - 80;
            if (snake->position[0].y >= HEIGHT - 40) snake->position[0].y = 40;
        }
            // Update direction of tail based on the segment before it
    if (snake->length > 1) {
        int tailIndex = snake->length - 1;
        int prevIndex = tailIndex - 1;
        // // Determine the direction of the tail based on the segment before it
        if (snake->position[prevIndex].x > snake->position[tailIndex].x) 
            {snake->recdir[tailIndex].x = 1;
                snake->recdir[tailIndex].y = 0; } // Previous segment is to the right
        if (snake->position[prevIndex].x < snake->position[tailIndex].x) 
            {snake->recdir[tailIndex].x = -1;
                snake->recdir[tailIndex].y = 0; } // Previous segment is to the left
        if (snake->position[prevIndex].y > snake->position[tailIndex].y) 
            {snake->recdir[tailIndex].y = 1;
                snake->recdir[tailIndex].x = 0; } // Previous segment is below
        if (snake->position[prevIndex].y < snake->position[tailIndex].y) 
            {snake->recdir[tailIndex].y = -1; 
                snake->recdir[tailIndex].x = 0; }// Previous segment is above
    }
        snake->previousDir = snake->direction;
    }
}

// Function to implement AI for computer-controlled snake
void updateComputerSnake() {
    if (!snake2.isAlive || mode != COMPUTER) return;
    
    // Simple AI: Try to find the nearest food
    float closestDistance = 1000000;
    Vector2 targetPosition = {0};
    
    // Find closest food
    for (int i = 0; i < MAX_FOODS; i++) {
        if (foods[i].active) {
            float dist = vector2distance(snake2.position[0], foods[i].position);
            if (dist < closestDistance) {
                closestDistance = dist;
                targetPosition = foods[i].position;
            }
        }
    }
    
    // Calculate direction to target
    Vector2 newDir = snake2.direction;
    
    // Decide whether to go horizontally or vertically first
    if (fabs(targetPosition.x - snake2.position[0].x) > fabs(targetPosition.y - snake2.position[0].y)) {
        // Move horizontally first
        if (targetPosition.x > snake2.position[0].x) newDir = (Vector2){1, 0};
        else if (targetPosition.x < snake2.position[0].x) newDir = (Vector2){-1, 0};
    } else {
        // Move vertically first
        if (targetPosition.y > snake2.position[0].y) newDir = (Vector2){0, 1};
        else if (targetPosition.y < snake2.position[0].y) newDir = (Vector2){0, -1};
    }
    
    // Avoid obstacles and boundaries
    Vector2 nextPos = (Vector2){
        snake2.position[0].x + newDir.x * SCALED_TILE_SIZE,
        snake2.position[0].y + newDir.y * SCALED_TILE_SIZE
    };
    
    // If next position would hit a wall or obstacle, try other directions
    if (!snake2.isGhost && (nextPos.x < 40 || nextPos.x >= WIDTH - 40 || 
        nextPos.y < 40 || nextPos.y >= HEIGHT - 40 || 
        isInsideObstacle(nextPos))) {
        
        // Try other directions (right, down, left, up)
        Vector2 directions[4] = {
            {1, 0}, {0, 1}, {-1, 0}, {0, -1}
        };
        
        bool foundSafeDirection = false;
        for (int i = 0; i < 4; i++) {
            // Skip opposite direction
            if (isOppositeDirection(directions[i], snake2.previousDir)) continue;
            
            nextPos = (Vector2){
                snake2.position[0].x + directions[i].x * SCALED_TILE_SIZE,
                snake2.position[0].y + directions[i].y * SCALED_TILE_SIZE
            };
            
            if (nextPos.x >= 40 && nextPos.x < WIDTH - 40 && 
                nextPos.y >= 40 && nextPos.y < HEIGHT - 40 && 
                !isInsideObstacle(nextPos)) {
                newDir = directions[i];
                foundSafeDirection = true;
                break;
            }
        }
        
        // If no safe direction found, continue in current direction
        if (!foundSafeDirection) {
            newDir = snake2.direction;
        }
    }
    
    // Also avoid self-collision
    for (int i = 1; i < snake2.length; i++) {
        nextPos = (Vector2){
            snake2.position[0].x + newDir.x * SCALED_TILE_SIZE,
            snake2.position[0].y + newDir.y * SCALED_TILE_SIZE
        };
        
        if (vector2distance(nextPos, snake2.position[i]) < SCALED_TILE_SIZE) {
            // Try to find another direction that doesn't collide
            Vector2 directions[4] = {
                {1, 0}, {0, 1}, {-1, 0}, {0, -1}
            };
            
            bool foundSafeDirection = false;
            for (int j = 0; j < 4; j++) {
                // Skip opposite direction
                if (isOppositeDirection(directions[j], snake2.previousDir)) continue;
                
                nextPos = (Vector2){
                    snake2.position[0].x + directions[j].x * SCALED_TILE_SIZE,
                    snake2.position[0].y + directions[j].y * SCALED_TILE_SIZE
                };
                
                bool collides = false;
                for (int k = 1; k < snake2.length; k++) {
                    if (vector2distance(nextPos, snake2.position[k]) < SCALED_TILE_SIZE) {
                        collides = true;
                        break;
                    }
                }
                
                if (!collides && nextPos.x >= 40 && nextPos.x < WIDTH - 40 && 
                    nextPos.y >= 40 && nextPos.y < HEIGHT - 40 && 
                    !isInsideObstacle(nextPos)) {
                    newDir = directions[j];
                    foundSafeDirection = true;
                    break;
                }
            }
            
            // If no safe direction found, try a random one
            if (!foundSafeDirection) {
                int randDir = rand() % 4;
                newDir = directions[randDir];
            }
            
            break;
        }
    }
    
    // Check for valid reverse
    if (isOppositeDirection(newDir, snake2.previousDir) && !snake2.reversing) {
        snake2.reversing = true;
        snake2.pivotPoint = snake2.length - 1;
        snake2.direction = newDir;
    } else if (!isOppositeDirection(newDir, snake2.previousDir)) {
        snake2.direction = newDir;
    }
}

// Function to draw the snake
void drawSnake(Snake snake) {
    if (!snake.isAlive) return;
    // Draw body segments
    for (int i =1; i < snake.length - 1; i++) {
        // Skip rendering if snake is flashing due to powerup
        if (snake.isPowerupActive && (int)(snake.flashTimer * 10) % 2 == 0) continue;
        
        //Rectangle sourceRec = {0.0f, 0.0f, (float)snake.texture.width, (float)snake.texture.height};
        //Rectangle destRec = {snake.position[i].x, snake.position[i].y, SCALED_TILE_SIZE, SCALED_TILE_SIZE};
        //Vector2 origin = {0.0f, 0.0f};
        
        // If snake is ghost, make it semi-transparent
        //Color segmentColor = snake.isGhost ? ColorAlpha(WHITE, 0.5f) : WHITE;
        
        //DrawTexturePro(snake.texture, sourceRec, destRec, origin, 0.0f, segmentColor);
        if(snake.headTexture.id!=snake2.headTexture.id){
        DrawRectangle(snake.position[i].x, snake.position[i].y, SCALED_TILE_SIZE, SCALED_TILE_SIZE, CLITERAL(Color){ 181, 230, 29, 255 });}
        else{
            DrawRectangle(snake.position[i].x, snake.position[i].y, SCALED_TILE_SIZE, SCALED_TILE_SIZE, CLITERAL(Color){ 252, 9, 9, 255 });}

        
    }
    
    // Draw head with rotation
    if (!(snake.isPowerupActive && (int)(snake.flashTimer * 10) % 2 == 0)) {
        // Rectangle sourceRec = {0.0f, 0.0f, (float)snake.headTexture.width, (float)snake.headTexture.height};
        Color headColor = snake.isGhost ? ColorAlpha(WHITE, 0.5f) : WHITE;
        frameRec.y = 0;
        //Rectangle destRec = {snake.position[0].x, snake.position[0].y, SCALED_TILE_SIZE, SCALED_TILE_SIZE};
        Vector2 origin = {SCALED_TILE_SIZE/2.0f, SCALED_TILE_SIZE/2.0f};
        float rotation = getHeadRotation(snake.direction);
        //Rectangle headrect;
        if (snake.direction.x==1){
            Rectangle headrect = {snake.position[0].x, snake.position[0].y, SCALED_TILE_SIZE+40, SCALED_TILE_SIZE+40 };
            DrawTexturePro(
                snake.headTexture, 
                frameRec, 
                headrect,
                origin, 
                rotation, 
                headColor
            );}
        if (snake.direction.x==-1){
            Rectangle headrect= {snake.position[0].x + (SCALED_TILE_SIZE+40)/2.0f, snake.position[0].y + (SCALED_TILE_SIZE+40)/2.0f, SCALED_TILE_SIZE+40, SCALED_TILE_SIZE+40 };
            DrawTexturePro(
                snake.headTexture, 
                frameRec, 
                headrect,
                origin, 
                rotation, 
                headColor
            );}
        if (snake.direction.y==1){
            Rectangle headrect= { snake.position[0].x + (SCALED_TILE_SIZE+40)/2.0f, snake.position[0].y, SCALED_TILE_SIZE+40, SCALED_TILE_SIZE+40 };
            DrawTexturePro(
                snake.headTexture, 
                frameRec, 
                headrect,
                origin, 
                rotation, 
                headColor
            );}
        if (snake.direction.y==-1){
            Rectangle headrect= {snake.position[0].x, snake.position[0].y + (SCALED_TILE_SIZE+40)/2.0f , SCALED_TILE_SIZE+40, SCALED_TILE_SIZE+40 };
            DrawTexturePro(
                snake.headTexture, 
                frameRec, 
                headrect,
                origin, 
                rotation, 
                headColor
            );}
            if (snake.length > 1)
        {
            int tailIndex = snake.length - 1;
            tailFrameRec.y = 0;
            Vector2 tailOrigin = { SCALED_TILE_SIZE, SCALED_TILE_SIZE};
            float tailRotation = 0.0f;
            Vector2 tailPosition;
            
            // Use the tail's direction for rendering (set in MoveSnake)

            
            if (snake.recdir[tailIndex].x == 1) {
                tailRotation = 180.0f;
                tailPosition = (Vector2){ snake.position[tailIndex].x, snake.position[tailIndex].y };
            }
            else if (snake.recdir[tailIndex].y == 1) {
                tailRotation = 270.0f;
                tailPosition = (Vector2){ snake.position[tailIndex].x + SCALED_TILE_SIZE, snake.position[tailIndex].y  };
            }
            else if (snake.recdir[tailIndex].x == -1) {
                tailRotation = 0.0f;
                tailPosition = (Vector2){ snake.position[tailIndex].x + SCALED_TILE_SIZE, snake.position[tailIndex].y + SCALED_TILE_SIZE };
            }
            else if (snake.recdir[tailIndex].y == -1) {
                tailRotation = 90.0f;
                tailPosition = (Vector2){ snake.position[tailIndex].x , snake.position[tailIndex].y + SCALED_TILE_SIZE };
            }
            
            DrawTexturePro(
                snake.tailTexture,
                tailFrameRec,
                (Rectangle){ tailPosition.x, tailPosition.y, SCALED_TILE_SIZE, SCALED_TILE_SIZE },
                tailOrigin,
                tailRotation,
                WHITE
            );
        }
        
    }
}

// Function to draw all foods
void drawFoods() {
    for (int i = 0; i < MAX_FOODS; i++) {
        if (foods[i].active) {
            Rectangle sourceRec = {0.0f, 0.0f, (float)foods[i].texture.width, (float)foods[i].texture.height};
            Rectangle destRec = {foods[i].position.x, foods[i].position.y, SCALED_TILE_SIZE, SCALED_TILE_SIZE};
            Vector2 origin = {0.0f, 0.0f};
            DrawTexturePro(foods[i].texture, sourceRec, destRec, origin, 0.0f, foods[i].color);
        }
    }
}

// Function to draw all bombs
void drawBombs() {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].active) {
            Rectangle sourceRec = {0.0f, 0.0f, (float)bombs[i].texture.width, (float)bombs[i].texture.height};
            Rectangle destRec = {bombs[i].position.x, bombs[i].position.y, SCALED_TILE_SIZE, SCALED_TILE_SIZE};
            Vector2 origin = {0.0f, 0.0f};
            DrawTexturePro(bombs[i].texture, sourceRec, destRec, origin, 0.0f, WHITE);
        }
    }
}

// Function to draw all powerups
void drawPowerups() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerups[i].active) {
            Rectangle sourceRec = {0.0f, 0.0f, (float)powerups[i].texture.width, (float)powerups[i].texture.height};
            Rectangle destRec = {powerups[i].position.x, powerups[i].position.y, SCALED_TILE_SIZE, SCALED_TILE_SIZE};
            Vector2 origin = {0.0f, 0.0f};
            DrawTexturePro(powerups[i].texture, sourceRec, destRec, origin, 0.0f, powerups[i].color);
        }
    }
}

// Function to draw obstacles
void drawObstacles() {
    for (int i = 0; i < obstacleCount; i++) {
        Rectangle sourceRec = {0.0f, 0.0f, (float)obstacles[i].texture.width, (float)obstacles[i].texture.height};
        DrawTexturePro(
            obstacles[i].texture,
            sourceRec,
            obstacles[i].bounds,
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
    }
}
