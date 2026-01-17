#include "types.h"
#include "globals.h"
#include "resources.h"
#include "storage.h"
#include "entities.h"
#include "ui.h"
#include "game.h"

// Main game loop
int main(void) {
    // Initialize window and audio
    InitWindow(WIDTH, HEIGHT, "SNAKEYYY");
    SetTargetFPS(8);
    InitAudioDevice();
    
    // Set random seed based on current time
    srand(time(NULL));
    
    // Initialize game state
    state = MAIN_MENU;
    loadResources();
    loadHighScores();
    initGameObjects();
    
    // Main game loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // Update game based on current state
        switch (state) {
            case MAIN_MENU:
                updateMainMenu();
                drawMainMenu();
                break;
                
            case GAMEPLAY:
                updateGameplay(deltaTime);
                drawGameplay();
                break;
                
            case PAUSE:
                updatePause();
                drawPause();
                break;
                
            case GAME_OVER:
                updateGameOver();
                drawGameOver();
                break;
                
            case HIGH_SCORES:
                updateHighScores();
                drawHighScores();
                break;
                
            case SETTINGS:
                updateSettings();
                drawSettings();
                break;
        }
    }
    
    // Save high scores before exiting
    saveHighScores();
    
    // Cleanup resources
    unloadResources();
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}
