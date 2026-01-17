#include "resources.h"

// Function to load textures and sounds
void loadResources() {
    // Textures
    snake1.texture = LoadTexture("assets/snakebody.png");
    snake1.headTexture = LoadTexture("assets/snake1.1spritesheet.png");
    snake1.tailTexture = LoadTexture("assets/tail1.1-removebg-preview (1).png");  // Load tail texture
    frameRec = (Rectangle){ 0.0f, 0.0f, (float)snake1.headTexture.width/4, (float)snake1.headTexture.height/2 };
    tailFrameRec = (Rectangle){ 0.0f, 0.0f, (float)snake1.tailTexture.width/6, (float)snake1.tailTexture.height/2 }; // Initialize tail frame
    snake2.texture = LoadTexture("assets/snake_body2.jpg");
    snake2.headTexture = LoadTexture("assets/snake2.1spritesheet.png");
    snake2.tailTexture = LoadTexture("assets/tail2.1-removebg-preview (1).png");
    
    for (int i = 0; i < MAX_FOODS; i++) {
        foods[i].texture = LoadTexture("assets/food.png");
        foods[i].active = false;
    }
    
    for (int i = 0; i < MAX_BOMBS; i++) {
        bombs[i].texture = LoadTexture("assets/bomb.png");
        bombs[i].active = false;
    }
    
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerups[i].texture = LoadTexture("assets/powerup.png");
        powerups[i].active = false;
    }
    
    backgroundTexture = LoadTexture("assets/background.png");
    obstacleTexture = LoadTexture("assets/obstacle.png");
    menuBackgroundTexture = LoadTexture("assets/menu_background.png");
    powerupTexture = LoadTexture("assets/powerup.png");
    
    // Sounds
    eatSound = LoadSound("assets/eat.mp3");
    crashSound = LoadSound("assets/crash.mp3");
    powerupSound = LoadSound("assets/powerup.mp3");
    bombSound = LoadSound("assets/bomb.mp3");
    levelSound = LoadSound("assets/levelup.mp3");
    gameMusic = LoadMusicStream("assets/game_music.mp3");
}

// Function to unload resources
void unloadResources() {
    UnloadTexture(snake1.texture);
    UnloadTexture(snake1.headTexture);
    UnloadTexture(snake2.texture);
    UnloadTexture(snake2.headTexture);
    UnloadTexture(snake1.tailTexture);
    UnloadTexture(snake2.tailTexture);
    for (int i = 0; i < MAX_FOODS; i++) {
        UnloadTexture(foods[i].texture);
    }
    
    for (int i = 0; i < MAX_BOMBS; i++) {
        UnloadTexture(bombs[i].texture);
    }
    
    for (int i = 0; i < MAX_POWERUPS; i++) {
        UnloadTexture(powerups[i].texture);
    }
    
    UnloadTexture(obstacleTexture);
    UnloadTexture(backgroundTexture);
    UnloadTexture(menuBackgroundTexture);
    
    UnloadSound(eatSound);
    UnloadSound(crashSound);
    UnloadSound(powerupSound);
    UnloadSound(bombSound);
    UnloadSound(levelSound);
    UnloadMusicStream(gameMusic);
}
