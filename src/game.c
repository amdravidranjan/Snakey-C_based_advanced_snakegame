#include "game.h"
#include "storage.h"

// Function to handle player input
void handleInput() {
    // Player 1 controls
    if (snake1.isAlive) {
        Vector2 newDir = snake1.direction;
        
        if (IsKeyPressed(KEY_RIGHT)) newDir = (Vector2){1, 0};
        if (IsKeyPressed(KEY_LEFT)) newDir = (Vector2){-1, 0};
        if (IsKeyPressed(KEY_UP)) newDir = (Vector2){0, -1};
        if (IsKeyPressed(KEY_DOWN)) newDir = (Vector2){0, 1};
        
        // Check for valid reverse
        if (isOppositeDirection(newDir, snake1.previousDir) && !snake1.reversing) {
            snake1.reversing = true;
            snake1.pivotPoint = snake1.length - 1;
            snake1.direction = newDir;
        } else if (!isOppositeDirection(newDir, snake1.previousDir)) {
            snake1.direction = newDir;
        }
    }
    
    // Player 2 controls in multiplayer
    if (mode == MULTIPLAYER && snake2.isAlive) {
        Vector2 newDir2 = snake2.direction;
        
        if (IsKeyPressed(KEY_D)) newDir2 = (Vector2){1, 0};
        if (IsKeyPressed(KEY_A)) newDir2 = (Vector2){-1, 0};
        if (IsKeyPressed(KEY_W)) newDir2 = (Vector2){0, -1};
        if (IsKeyPressed(KEY_S)) newDir2 = (Vector2){0, 1};
        
        // Check for valid reverse
        if (isOppositeDirection(newDir2, snake2.previousDir) && !snake2.reversing) {
            snake2.reversing = true;
            snake2.pivotPoint = snake2.length - 1;
            snake2.direction = newDir2;
        } else if (!isOppositeDirection(newDir2, snake2.previousDir)) {
            snake2.direction = newDir2;
        }
    }
}

// Function to check collisions
void checkCollisions() {
    // Check snake1 collisions
    if (snake1.isAlive) {
        Vector2 head = snake1.position[0];
        
        // Check wall collisions
        if (!snake1.isGhost && (head.x < 40 || head.x >= WIDTH - 40 || head.y < 40 || head.y >= HEIGHT - 40)) {
            if (!snake1.isInvincible) {
                snake1.isAlive = false;
                if (soundEnabled) PlaySound(crashSound);
            }
        }
        
        // Check obstacle collisions
        if (!snake1.isGhost && isInsideObstacle(head)) {
            if (!snake1.isInvincible) {
                snake1.isAlive = false;
                if (soundEnabled) PlaySound(crashSound);
            }
        }
        
        // Check self-collision
        if (!snake1.reversing) {
        for (int i = 1; i < snake1.length; i++) {
            if (vector2distance(head, snake1.position[i]) < SCALED_TILE_SIZE / 2) {
                if (!snake1.isInvincible) {
                    snake1.isAlive = false;
                    if (soundEnabled) PlaySound(crashSound);
                }
                break;
            }
        }}
        
        // Check collision with snake2
        if ((mode == MULTIPLAYER || mode == COMPUTER) && snake2.isAlive) {
            for (int i = 0; i < snake2.length; i++) {
                if (vector2distance(head, snake2.position[i]) < SCALED_TILE_SIZE / 2) {
                    if (!snake1.isInvincible) {
                        snake1.isAlive = false;
                        if (soundEnabled) PlaySound(crashSound);
                    }
                    break;
                }
            }
        }
        
        // Check food collisions
        for (int i = 0; i < MAX_FOODS; i++) {
            if (foods[i].active && vector2distance(head, foods[i].position) < SCALED_TILE_SIZE) {
                // Eat food
                foods[i].active = false;
                
                // Add points
                int points = foods[i].points;
                if (snake1.isPowerupActive && snake1.powerupTimer > 0) {
                    // Double points if powerup active
                    points *= 2;
                }
                
                snake1.score += points;
                
                // Grow snake
                if (snake1.length < MAX_SNAKE_LENGTH) {
                    snake1.length++;
                    snake1.position[snake1.length-1] = snake1.position[snake1.length - 2];
                }
                
                // Play sound
                if (soundEnabled) PlaySound(eatSound);
                
                // Generate new food
                generateNewFood();
                
                // Increase difficulty
                if (snake1.score % 10 == 0) {
                    level++;
                    if (soundEnabled) PlaySound(levelSound);
                    
                    // Increase game elements based on level
                    if (level % 2 == 0 && foodCount < MAX_FOODS) {
                        foodCount++;
                        generateNewFood();
                    }
                    
                    if (level % 3 == 0 && bombCount < MAX_BOMBS) {
                        bombCount++;
                        generateNewBomb();
                    }
                    
                    // Generate new obstacles in survival mode
                    if (mode == SURVIVAL || mode == MAZE) {
                        generateObstacles();
                    }
                    
                    // Speed up snake
                    snake1.baseInterval = fmax(0.05f, snake1.baseInterval - 0.005f);
                    if (!snake1.isPowerupActive) {
                        snake1.moveInterval = snake1.baseInterval;
                    }
                    
                    if (mode == MULTIPLAYER || mode == COMPUTER) {
                        snake2.baseInterval = fmax(0.05f, snake2.baseInterval - 0.005f);
                        if (!snake2.isPowerupActive) {
                            snake2.moveInterval = snake2.baseInterval;
                        }
                    }
                }
            }
        }
        
        // Check bomb collisions
        for (int i = 0; i < MAX_BOMBS; i++) {
            if (bombs[i].active && vector2distance(head, bombs[i].position) < SCALED_TILE_SIZE) {
                bombs[i].active = false;
                
                if (!snake1.isInvincible) {
                    // Lose points and shrink snake
                    snake1.score = fmax(0, snake1.score - 5);
                    snake1.length = fmax(3, snake1.length - 2);
                    
                    // Play sound
                    if (soundEnabled) PlaySound(bombSound);
                }
                
                // Generate new bomb
                generateNewBomb();
            }
        }
        
        // Check powerup collisions
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (powerups[i].active && vector2distance(head, powerups[i].position) < SCALED_TILE_SIZE) {
                powerups[i].active = false;
                
                // Apply powerup effect
                snake1.isPowerupActive = true;
                snake1.powerupTimer = powerups[i].duration;
                snake1.flashTimer = 0.0f;
                
                switch (powerups[i].type) {
                    case SPEED:
                        snake1.moveInterval = snake1.baseInterval * 0.5f;
                        break;
                    case INVINCIBLE:
                        snake1.isInvincible = true;
                        break;
                    case DOUBLE_POINTS:
                        // Effect applied when eating food
                        break;
                    case GHOST:
                        snake1.isGhost = true;
                        break;
                }
                
                // Play sound
                if (soundEnabled) PlaySound(powerupSound);
            }
        }
    }
    
    // Repeat similar checks for snake2 if applicable
    if ((mode == MULTIPLAYER || mode == COMPUTER) && snake2.isAlive) {
        Vector2 head = snake2.position[0];
        
        // Check wall collisions
        if (!snake2.isGhost && (head.x < 40 || head.x > WIDTH - 80 || head.y < 40 || head.y > HEIGHT - 40)) {
            if (!snake2.isInvincible) {
                snake2.isAlive = false;
                if (soundEnabled) PlaySound(crashSound);
            }
        }
        
        // Check obstacle collisions
        if (!snake2.isGhost && isInsideObstacle(head)) {
            if (!snake2.isInvincible) {
                snake2.isAlive = false;
                if (soundEnabled) PlaySound(crashSound);
            }
        }
        
        // Check self-collision
        if (!snake2.reversing){for (int i = 1; i < snake2.length; i++) {
            if (vector2distance(head, snake2.position[i]) < SCALED_TILE_SIZE / 2) {
                if (!snake2.isInvincible) {
                    snake2.isAlive = false;
                    if (soundEnabled) PlaySound(crashSound);
                }
                break;
            }
        }}
        
        // Check collision with snake1
        if (snake1.isAlive) {
            for (int i = 0; i < snake1.length; i++) {
                if (vector2distance(head, snake1.position[i]) < SCALED_TILE_SIZE / 2) {
                    if (!snake2.isInvincible) {
                        snake2.isAlive = false;
                        if (soundEnabled) PlaySound(crashSound);
                    }
                    break;
                }
            }
        }
        
        // Check food collisions (similar to snake1)
        for (int i = 0; i < MAX_FOODS; i++) {
            if (foods[i].active && vector2distance(head, foods[i].position) < SCALED_TILE_SIZE) {
                foods[i].active = false;
                
                int points = foods[i].points;
                if (snake2.isPowerupActive && snake2.powerupTimer > 0) {
                    points *= 2;
                }
                
                snake2.score += points;
                
                if (snake2.length < MAX_SNAKE_LENGTH) {
                    snake2.length++;
                    snake2.position[snake2.length-1] = snake2.position[snake2.length - 2];
                }
                
                if (soundEnabled) PlaySound(eatSound);
                generateNewFood();
            }
        }
        
        // Check bomb collisions (similar to snake1)
        for (int i = 0; i < MAX_BOMBS; i++) {
            if (bombs[i].active && vector2distance(head, bombs[i].position) < SCALED_TILE_SIZE) {
                bombs[i].active = false;
                
                if (!snake2.isInvincible) {
                    snake2.score = fmax(0, snake2.score - 5);
                    snake2.length = fmax(3, snake2.length - 2);
                    if (soundEnabled) PlaySound(bombSound);
                }
                
                generateNewBomb();
            }
        }
        
        // Check powerup collisions (similar to snake1)
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (powerups[i].active && vector2distance(head, powerups[i].position) < SCALED_TILE_SIZE) {
                powerups[i].active = false;
                
                snake2.isPowerupActive = true;
                snake2.powerupTimer = powerups[i].duration;
                snake2.flashTimer = 0.0f;
                
                switch (powerups[i].type) {
                    case SPEED:
                        snake2.moveInterval = snake2.baseInterval * 0.5f;
                        break;
                    case INVINCIBLE:
                        snake2.isInvincible = true;
                        break;
                    case DOUBLE_POINTS:
                        // Effect applied when eating food
                        break;
                    case GHOST:
                        snake2.isGhost = true;
                        break;
                }
                
                if (soundEnabled) PlaySound(powerupSound);
            }
        }
    }
    
    // Check for game over
    if (!snake1.isAlive && (mode != MULTIPLAYER && mode != COMPUTER || !snake2.isAlive)) {
        state = GAME_OVER;
        
        // Check if score qualifies for high score
        int lowestScore = 0;
        if (MAX_HIGH_SCORES > 0) {
            lowestScore = highScores[MAX_HIGH_SCORES-1].score;
        }
        
        if (snake1.score > lowestScore || snake2.score > lowestScore) {
            // We'll handle high score entry in the game over screen
            for(int i =0; i< MAX_HIGH_SCORES; i++){
                
            if (highScores[i].score < snake1.score){
                for (int j =3; j>=i; j--){highScores[j+1].score=highScores[j].score;}
            highScores[i].score=snake1.score;strcpy(highScores[i].name, playerName); break; 
            }
            else if (highScores[i].score < snake2.score){
                for (int j =3; j>=i; j--){highScores[j+1].score=highScores[j].score;}
            highScores[i].score=snake2.score;
            strcpy(highScores[i].name, playerName); break;
             }
        }
    }
}
}

// Function to draw the game
void drawGameplay() {
    BeginDrawing();
    Rectangle sourceRec = {0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height};
    Rectangle destRec = {0.0f, 0.0f, WIDTH, HEIGHT};
    Vector2 origin={0.0, 0.0};
    if (nightMode) {
        ClearBackground(BLACK);
    } else {
        ClearBackground(RAYWHITE);
       // DrawTexture(backgroundTexture, 0, 0, Fade(WHITE, 0.3f));
       DrawTexturePro(backgroundTexture, sourceRec, destRec, origin, 0.0f, WHITE);
    }
    
    // Draw grid if enabled
    drawGrid();
    
    // Draw boundaries
    DrawRectangle(0, 0, WIDTH, 40, GRAY);
    DrawRectangle(0, HEIGHT - 40, WIDTH, 40, GRAY);
    DrawRectangle(0, 0, 40, HEIGHT, GRAY);
    DrawRectangle(WIDTH - 40, 0, 40, HEIGHT, GRAY);
    
    // Draw obstacles
    drawObstacles();
    
    // Draw foods, bombs, powerups
    drawFoods();
    drawBombs();
    drawPowerups();
    
    // Draw snakes
    drawSnake(snake1);
    if (mode == MULTIPLAYER || mode == COMPUTER) {
        drawSnake(snake2);
    }
    
    // Draw UI elements
    int uiY = 25;
    Color textColor = nightMode ? WHITE : BLACK;
    
    // Draw player 1 info
    DrawText(TextFormat("P1 Score: %d", snake1.score), 10, uiY, 20, textColor);
    
    // Draw player 2 info if applicable
    if (mode == MULTIPLAYER || mode == COMPUTER) {
        DrawText(TextFormat("P2 Score: %d", snake2.score), WIDTH - 150, uiY, 20, textColor);
    }
    
    // Draw level and time info
    DrawText(TextFormat("Level: %d", level), 10, uiY + 25, 20, textColor);
    DrawText(TextFormat("Time: %.0f", gameTimer), WIDTH - 150, uiY + 25, 20, textColor);
    
    // Draw active powerups
    if (snake1.isPowerupActive) {
        if (snake1.isPowerupActive) {
        const char* powerupName = "";
        if (snake1.isInvincible) powerupName = "Invincible";
        else if (snake1.isGhost) powerupName = "Ghost";
        else if (snake1.moveInterval < snake1.baseInterval) powerupName = "Speed";
        else powerupName = "Double Points";
        
        DrawText(TextFormat("P1 Powerup: %s (%.1f)", powerupName, snake1.powerupTimer), 
                 200, uiY, 20, GOLD);
    }
//        DrawText(TextFormat("%s", power), 200, uiY, 20, GOLD);
//        DrawText(TextFormat("%s POWERUP: %.1f", power, snake1.powerupTimer), 300, uiY, 20, GOLD);
    }

    
    if ((mode == MULTIPLAYER || mode == COMPUTER) && snake2.isPowerupActive) {
        DrawText(TextFormat("P2 POWERUP: %.1f", snake2.powerupTimer), 200, uiY + 25, 20, BLUE);
    }
    
    // Draw pause hint
    DrawText("Press P to Pause", WIDTH/2 - 80, HEIGHT - 40, 20, textColor);
    
    EndDrawing();
}

// Function to update gameplay
void updateGameplay(float deltaTime) {
    if (IsKeyPressed(KEY_P)) {
        state = PAUSE;
        previousState = GAMEPLAY;
        return;
    }
    
    gameTimer += deltaTime;
    bombTimer += deltaTime;
    powerupTimer += deltaTime;
    
    // Handle player input
    handleInput();
    
    // Update computer-controlled snake if applicable
    if (mode == COMPUTER) {
        updateComputerSnake();
    }
    
    // Update snake movements
    updateSnake(&snake1, deltaTime);
    if (mode == MULTIPLAYER || mode == COMPUTER) {
        updateSnake(&snake2, deltaTime);
    }
    
    // Check for collisions
    checkCollisions();
    
    // Spawn additional game elements
    if (bombTimer >= 15.0f && bombCount < MAX_BOMBS) {
        generateNewBomb();
        bombTimer = 0.0f;
    }
    
    if (powerupTimer >= 20.0f) {
        generateNewPowerup();
        powerupTimer = 0.0f;
    }
    
    
    // Play music if enabled
    if (musicEnabled) {
        UpdateMusicStream(gameMusic);
        PlayMusicStream(gameMusic);
        
        
    }
    framesCounter++;
    
    // if (framesCounter >= (60/framesSpeed))
    // {
    //     framesCounter = 0;
    //     currentFrame++;
        
    //     if (currentFrame > 3) currentFrame = 0;
        
    //     frameRec.x = (float)currentFrame * (float)snake1.headTexture.width/4;
    // }
    // if (framesCounter >= (60/tailSpeed))
    // {
    //     framesCounter = 0;
    //     tailCurrentFrame++;  // Update tail frame
    //     
    //     if (tailCurrentFrame > 5) tailCurrentFrame = 0;  // Reset tail frame (note: 6 columns)
    //     
    //     tailFrameRec.x = (float)tailCurrentFrame * (float)snake1.tailTexture.width/6;  // Update tail frame x position
    // }
    if (framesCounter >= (60/framesSpeed))
    {
        framesCounter = 0;
        currentFrame++;
        tailCurrentFrame++;  // Update tail frame
        
        if (currentFrame > 3) currentFrame = 0;
        if (tailCurrentFrame > 5) tailCurrentFrame = 0;  // Reset tail frame (note: 6 columns)
        
        frameRec.x = (float)currentFrame * (float)snake1.headTexture.width/4;
        tailFrameRec.x = (float)tailCurrentFrame * (float)snake1.tailTexture.width/6;  // Update tail frame x position
    }
}
