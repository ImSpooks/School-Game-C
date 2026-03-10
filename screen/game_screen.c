#include "game_screen.h"

#include <stddef.h>

#include "../hud.h"

void initializeGameScreen();
void unloadGameScreen();
void drawGameScreen(const RenderTexture2D* texture);
void updateGameScreen(const RenderTexture2D* texture);

Screen gameScreen = {
    .initialize = initializeGameScreen,
    .unload = unloadGameScreen,
    .update = drawGameScreen,
    .draw = updateGameScreen,
};

Level* currentLevel = NULL;
Level* newLevel = NULL;

bool swappingLevel = false;
bool fadeIn = false;
static float timer = 0;

void initializeGameScreen() {
    setLevel(&levelStart);
}

void unloadGameScreen() {
    if (currentLevel != NULL) {
        currentLevel->unload();
    }
}

void drawGameScreen(const RenderTexture2D* texture) {
    if (currentLevel != NULL) {
        DrawTexturePro(currentLevel->texture,
            (Rectangle) {0, 0, (float) currentLevel->texture.width, (float) currentLevel->texture.height},
            (Rectangle) {0, 0, (float) texture->texture.width, (float) texture->texture.height},
            (Vector2) {0, 0},
            0.0f,
            WHITE);
    }

    if (swappingLevel) {
        int alpha;
        if (fadeIn) {
            alpha = (int) (timer * TRANSITION_LENGTH * 255);
        } else {
            alpha = (int) ((TRANSITION_LENGTH - timer) * TRANSITION_LENGTH * 255);
        }
        DrawRectangle(0, 0, GetScreenHeight(), GetScreenHeight(), (Color) {0, 0, 0, (unsigned char) alpha});
    }
}

void updateGameScreen(const RenderTexture2D* texture) {
    if (swappingLevel) {
        timer += GetFrameTime();

        if (timer > TRANSITION_LENGTH) {
            swappingLevel = false;
            currentLevel->load();
        }
        if (timer > TRANSITION_LENGTH / 2 && fadeIn) {
            fadeIn = false;

            setLevel(newLevel);
            newLevel = NULL;
        }
    }
}

Level* getLevel() {
    return currentLevel;
}

void changeLevel(Level* level) {
    setButtons(NULL, 0);
    if (swappingLevel) {
        return;
    }
    newLevel = level;
    fadeIn = true;
    swappingLevel = true;
}

void setLevel(Level* level) {
    if (currentLevel == level) {
        return;
    }

    const Level* prevLevel = currentLevel;

    currentLevel = level;

    if (prevLevel != NULL && prevLevel != currentLevel) {
        prevLevel->unload();
    }
}