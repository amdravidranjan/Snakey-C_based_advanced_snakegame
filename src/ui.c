#include "ui.h"
#include "storage.h" // For saving high scores

// Draw background grid
void drawGrid() {
    if (!showGrid) return;
    
    for (int i = 0; i < WIDTH / SCALED_TILE_SIZE; i++) {
        DrawLine(i * SCALED_TILE_SIZE, 0, i * SCALED_TILE_SIZE, HEIGHT, Fade(LIGHTGRAY, 0.3f));
    }
    
    for (int i = 0; i < HEIGHT / SCALED_TILE_SIZE; i++) {
        DrawLine(0, i * SCALED_TILE_SIZE, WIDTH, i * SCALED_TILE_SIZE, Fade(LIGHTGRAY, 0.3f));
    }
}

// Function to update main menu
void updateMainMenu() {
    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption = (selectedOption + 1) % 7;
    } else if (IsKeyPressed(KEY_UP)) {
        selectedOption = (selectedOption - 1 + 7) % 7;
    }
    
    if (IsKeyPressed(KEY_ENTER)) {
        switch (selectedOption) {
            case 0: // Classic Mode
                mode = CLASSIC;
                state = GAMEPLAY;
                initGameObjects();
                break;
            case 1: // Multiplayer
                mode = MULTIPLAYER;
                state = GAMEPLAY;
                initGameObjects();
                break;
            case 2: // Computer
                mode = COMPUTER;
                state = GAMEPLAY;
                initGameObjects();
                break;
            case 3: // Survival
                mode = SURVIVAL;
                state = GAMEPLAY;
                initGameObjects();
                break;
            case 4: // Maze
                mode = MAZE;
                state = GAMEPLAY;
                initGameObjects();
                break;
            case 5: // High Scores
                state = HIGH_SCORES;
                break;
            case 6: // Settings
                state = SETTINGS;
                break;
        }
    }
}

// Function to draw the main menu
void drawMainMenu() {
    BeginDrawing();
    Rectangle sourceRec = {0.0f, 0.0f, (float)menuBackgroundTexture.width, (float)menuBackgroundTexture.height};
    Rectangle destRec = {0.0f, 0.0f , WIDTH, HEIGHT};
    Vector2 origin = {0.0f, 0.0f};
    ClearBackground(RAYWHITE);
    DrawTexturePro(menuBackgroundTexture, sourceRec, destRec, origin, 0.0f, WHITE);
    
    DrawText("SNAKEYYYY", WIDTH/2 - MeasureText("SNAKEYYYY", 60)/2, 60, 60, DARKGREEN);
    DrawText("Choose a Game Mode:", WIDTH/2 - MeasureText("Choose a Game Mode:", 30)/2, 150, 30, DARKGREEN);
    
    const char* options[] = {
        "Classic Mode",
        "Multiplayer",
        "vs Computer",
        "Survival Mode",
        "Maze Mode",
        "High Scores",
        "Settings"
    };
    
    for (int i = 0; i < 7; i++) {
        Color optionColor = (i == selectedOption) ? RED : DARKGREEN;
        DrawText(options[i], WIDTH/2 - MeasureText(options[i], 20)/2, 220 + i*40, 20, optionColor);
    }
    
    DrawText("Use UP/DOWN to select, ENTER to confirm", WIDTH/2 - MeasureText("Use UP/DOWN to select, ENTER to confirm", 15)/2, HEIGHT - 100, 15, DARKGREEN);
    
    EndDrawing();
}

// Function to update game over screen
void updateGameOver() {
    if (IsKeyPressed(KEY_ENTER)) {
        state = MAIN_MENU;
    }
}

// Function to draw game over screen
void drawGameOver() {
    BeginDrawing();
    
    if (nightMode) {
        ClearBackground(BLACK);
    } else {
        
        Rectangle sourceRec = {0.0f, 0.0f, (float)menuBackgroundTexture.width, (float)menuBackgroundTexture.height};
        Rectangle destRec = {0.0f, 0.0f , WIDTH, HEIGHT};
        Vector2 origin = {0.0f, 0.0f};
        ClearBackground(RAYWHITE);
        DrawTexturePro(menuBackgroundTexture, sourceRec, destRec, origin, 0.0f, Fade(WHITE, 0.3f));
    }
    
    DrawText("GAME OVER", WIDTH/2 - MeasureText("GAME OVER", 60)/2, 100, 60, RED);
    
    // Different text based on game mode
    if (mode == MULTIPLAYER) {
        if (snake1.score > snake2.score) {
            DrawText("Player 1 Wins!", WIDTH/2 - MeasureText("Player 1 Wins!", 40)/2, 200, 40, GREEN);
        } else if (snake2.score > snake1.score) {
            DrawText("Player 2 Wins!", WIDTH/2 - MeasureText("Player 2 Wins!", 40)/2, 200, 40, BLUE);
        } else {
            DrawText("It's a Tie!", WIDTH/2 - MeasureText("It's a Tie!", 40)/2, 200, 40, PURPLE);
        }
    } else if (mode == COMPUTER) {
        if (snake1.score > snake2.score) {
            DrawText("You Win!", WIDTH/2 - MeasureText("You Win!", 40)/2, 200, 40, GREEN);
        } else {
            DrawText("Computer Wins!", WIDTH/2 - MeasureText("Computer Wins!", 40)/2, 200, 40, RED);
        }
    }
    
    // Display scores
    DrawText(TextFormat("Player 1 Score: %d", snake1.score), WIDTH/2 - 150, 280, 30, GREEN);
    if (mode == MULTIPLAYER || mode == COMPUTER) {
        DrawText(TextFormat("Player 2 Score: %d", snake2.score), WIDTH/2 - 150, 320, 30, BLUE);
    }
    
    DrawText(TextFormat("Level Reached: %d", level), WIDTH/2 - 150, 360, 30, PURPLE);
    DrawText(TextFormat("Time Survived: %.1f seconds", gameTimer), WIDTH/2 - 150, 400, 30, GOLD);
    
    DrawText("Press ENTER to return to Main Menu", WIDTH/2 - MeasureText("Press ENTER to return to Main Menu", 20)/2, HEIGHT - 50, 20, DARKGREEN);
    
    EndDrawing();
}

// Function to update pause screen
void updatePause() {
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ENTER)) {
        state = previousState;
    }
    
    if (IsKeyPressed(KEY_M)) {
        state = MAIN_MENU;
    }
}

// Function to draw pause screen
void drawPause() {
    BeginDrawing();
    
    if (nightMode) {
        ClearBackground(Fade(BLACK, 0.8f));
    } else {
        ClearBackground(Fade(RAYWHITE, 0.8f));
    }
    
    DrawText("GAME PAUSED", WIDTH/2 - MeasureText("GAME PAUSED", 40)/2, HEIGHT/2 - 60, 40, BLUE);
    DrawText("Press P or ENTER to resume", WIDTH/2 - MeasureText("Press P or ENTER to resume", 20)/2, HEIGHT/2, 20, DARKBLUE);
    DrawText("Press M to return to main menu", WIDTH/2 - MeasureText("Press M to return to main menu", 20)/2, HEIGHT/2 + 30, 20, DARKBLUE);
    
    EndDrawing();
}

// Function to update settings screen
void updateSettings() {
    if (IsKeyPressed(KEY_DOWN)) {
        selectedOption = (selectedOption + 1) % 6;
    } else if (IsKeyPressed(KEY_UP)) {
        selectedOption = (selectedOption - 1 + 6) % 6;
    }
    
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT)) {
        switch (selectedOption) {
            case 0: // Toggle Sound
                soundEnabled = !soundEnabled;
                break;
            case 1: // Toggle Music
                musicEnabled = !musicEnabled;
                if (musicEnabled) {
                    PlayMusicStream(gameMusic);
                } else {
                    StopMusicStream(gameMusic);
                }
                break;
            case 2: // Toggle Grid
                showGrid = !showGrid;
                break;
            case 3: // Toggle Night Mode
                nightMode = !nightMode;
                break;
            case 4: // Change Player Name
                // This would typically open a text input field
                // For simplicity, we'll just toggle between a few preset names
                if (strcmp(playerName, "Player") == 0) {
                    strcpy(playerName, "Snake Master");
                } else if (strcmp(playerName, "Snake Master") == 0) {
                    strcpy(playerName, "Viper");
                } else {
                    strcpy(playerName, "Player");
                }
                break;
            case 5: // Back to Main Menu
                state = MAIN_MENU;
                break;
        }
    }
}

// Function to draw settings screen
void drawSettings() {
    BeginDrawing();
    
    if (nightMode) {
        ClearBackground(BLACK);
    } else {
        
        Rectangle sourceRec = {0.0f, 0.0f, (float)menuBackgroundTexture.width, (float)menuBackgroundTexture.height};
        Rectangle destRec = {0.0f, 0.0f , WIDTH, HEIGHT};
        Vector2 origin = {0.0f, 0.0f};
        ClearBackground(RAYWHITE);
        DrawTexturePro(menuBackgroundTexture, sourceRec, destRec, origin, 0.0f, Fade(WHITE, 0.3f));
    }
    
    Color textColor = nightMode ? WHITE : BLACK;
    
    DrawText("SETTINGS", WIDTH/2 - MeasureText("SETTINGS", 40)/2, 60, 40, GREEN);
    
    const char* options[] = {
        TextFormat("Sound: %s", soundEnabled ? "ON" : "OFF"),
        TextFormat("Music: %s", musicEnabled ? "ON" : "OFF"),
        TextFormat("Show Grid: %s", showGrid ? "ON" : "OFF"),
        TextFormat("Night Mode: %s", nightMode ? "ON" : "OFF"),
        TextFormat("Player Name: %s", playerName),
        "Back to Main Menu"
    };
    
    for (int i = 0; i < 6; i++) {
        Color optionColor = (i == selectedOption) ? RED : DARKGREEN;
        DrawText(options[i], WIDTH/2 - 150, 150 + i*50, 20, optionColor);
    }
    
    DrawText("Use UP/DOWN to select, ENTER to toggle", WIDTH/2 - MeasureText("Use UP/DOWN to select, ENTER to toggle", 15)/2, HEIGHT - 50, 15, textColor);
    
    EndDrawing();
}

// Function to update high scores screen
void updateHighScores() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        state = MAIN_MENU;
    }
}

// Function to draw high scores screen
void drawHighScores() {
    BeginDrawing();
    
    if (nightMode) {
        ClearBackground(BLACK);
    } else {
        Rectangle sourceRec = {0.0f, 0.0f, (float)menuBackgroundTexture.width, (float)menuBackgroundTexture.height};
        Rectangle destRec = {0.0f, 0.0f , WIDTH, HEIGHT};
        Vector2 origin = {0.0f, 0.0f};
        ClearBackground(RAYWHITE);
        DrawTexturePro(menuBackgroundTexture, sourceRec, destRec, origin, 0.0f, Fade(WHITE, 0.3f));
    }
    
    Color textColor = nightMode ? WHITE : BLACK;
    
    DrawText("HIGH SCORES", WIDTH/2 - MeasureText("HIGH SCORES", 40)/2, 60, 40, GOLD);
    
    // Display high scores
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        if (highScores[i].score > 0) {
            const char* modeName = "";
            switch (highScores[i].mode) {
                case CLASSIC: modeName = "Classic"; break;
                case MULTIPLAYER: modeName = "Multiplayer"; break;
                case COMPUTER: modeName = "vs Computer"; break;
                case SURVIVAL: modeName = "Survival"; break;
                case MAZE: modeName = "Maze"; break;
            }
            
            DrawText(TextFormat("%d. %s", i+1, highScores[i].name), WIDTH/2 - 200, 150 + i*50, 20, PURPLE);
            DrawText(TextFormat("%d", highScores[i].score), WIDTH/2 + 50, 150 + i*50, 20, GOLD);
            DrawText(modeName, WIDTH/2 + 120, 150 + i*50, 20, BLUE);
        } else {
            DrawText(TextFormat("%d. ---", i+1), WIDTH/2 - 200, 150 + i*50, 20, GRAY);
            DrawText("0", WIDTH/2 + 50, 150 + i*50, 20, GRAY);
            DrawText("---", WIDTH/2 + 120, 150 + i*50, 20, GRAY);
        }
    }
    
    DrawText("Press ENTER to return to Main Menu", WIDTH/2 - MeasureText("Press ENTER to return to Main Menu", 20)/2, HEIGHT - 50, 20, textColor);
    
    EndDrawing();
}
