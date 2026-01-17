#include "storage.h"

// Function to save high scores to file
void saveHighScores(void) {
    FILE *file = fopen("highscores.txt", "wb");
    if (file) {
        fwrite(highScores, sizeof(HighScore), MAX_HIGH_SCORES, file);
        fclose(file);
    }
}

// Function to load high scores from file
void loadHighScores(void) {
    FILE *file = fopen("highscores.txt", "rb");
    
    // Initialize with default empty scores if file doesn't exist
    if (!file) {
        for (int i = 0; i < MAX_HIGH_SCORES; i++) {
            strcpy(highScores[i].name, "---");
            highScores[i].score = 0;
            highScores[i].mode = CLASSIC;
        }
        return;
    }
    
    // Read high scores from file
    fread(highScores, sizeof(HighScore), MAX_HIGH_SCORES, file);
    fclose(file);
}

void addHighScore(int score, GameMode scoreMode) {
    // Find where this score fits in the ranking
    int position = MAX_HIGH_SCORES;
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        if (score > highScores[i].score) {
            position = i;
            break;
        }
    }
    
    // If the score is good enough to be in the top MAX_HIGH_SCORES
    if (position < MAX_HIGH_SCORES) {
        // Shift lower scores down
        for (int i = MAX_HIGH_SCORES - 1; i > position; i--) {
            highScores[i] = highScores[i-1];
        }
        
        // Insert the new score
        strcpy(highScores[position].name, playerName);
        highScores[position].score = score;
        highScores[position].mode = scoreMode;
        
        saveHighScores();
    }
}
