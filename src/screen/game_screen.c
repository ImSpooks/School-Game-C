#include "screen/game_screen.h"

#include <stddef.h>
#include <stdio.h>

#include "globals.h"
#include "hud.h"
#include "asset_manager.h"

void initializeGameScreen();
void unloadGameScreen();
void drawGameScreen(const RenderTexture2D* texture);
void updateGameScreen(const RenderTexture2D* texture);

Screen gameScreen = {
    .initialize = initializeGameScreen,
    .unload = unloadGameScreen,
    .update = updateGameScreen,
    .draw = drawGameScreen,
};

Level* currentLevel = NULL;
Level* newLevel = NULL;

bool swappingLevel = false;
bool fadeIn = false;
static float timer = 0;

Music* music_current = NULL;

void initializeGameScreen() {
    levelStart.music = &assets.music_forest;
    levelForest1.music = &assets.music_forest;
    levelForest2.music = &assets.music_forest;
    levelForest3.music = &assets.music_forest;
    levelWizard1.music = &assets.music_forest;
    levelWizard2.music = &assets.music_forest;
    levelWizard3.music = &assets.music_forest;

    levelVillage1.music = &assets.music_village;
    levelVillage2.music = &assets.music_village;
    levelVillage3.music = &assets.music_village;
    levelVillage4.music = &assets.music_village;


    if (currentLevel == NULL) {
        setLevel(&levelStart);
    } else {
        currentLevel->load();
        music_current = currentLevel->music;
        if (!IsMusicStreamPlaying(*music_current)){
            PlayMusicStream(*music_current);
        }
    }

}

void unloadGameScreen() {
    if (currentLevel != NULL) {
        currentLevel->unload();
    }

    if (music_current != NULL) {
        StopMusicStream(*music_current);
        music_current = NULL;
    }
}

void drawGameScreen(const RenderTexture2D* texture) {
    if (currentLevel != NULL) {
        DrawTexturePro(*currentLevel->texture,
            (Rectangle) {0, 0, (float) (*currentLevel->texture).width, (float) (*currentLevel->texture).height},
            (Rectangle) {0, 0, (float) SCREEN_WIDTH, (float) SCREEN_HEIGHT},
            (Vector2) {0, 0},
            0.0f,
            WHITE);
    }

    if (swappingLevel) {
        int alpha;
        if (fadeIn) {
            alpha = (int) ((timer * 2) / TRANSITION_LENGTH * 255);
        } else {
            alpha = 255 - (int) ((timer * 2) / TRANSITION_LENGTH * 255);
        }
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {0, 0, 0, (unsigned char) alpha});
    }
}

void updateGameScreen(const RenderTexture2D* texture) {
    if (music_current != NULL)
        UpdateMusicStream(*music_current);

    if (swappingLevel) {
        timer += GetFrameTime();

        if (timer > TRANSITION_LENGTH) {
            swappingLevel = false;
            hud.render_buttons = true;
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
    if (TRANSITION_LENGTH == 0) {
        setLevel(level);
        return;
    }

    setButtons(NULL, 0);
    if (swappingLevel) {
        return;
    }
    hud.render_buttons = false;
    timer = 0;
    newLevel = level;
    fadeIn = true;
    swappingLevel = true;
}

void setLevel(Level* level) {
    if (currentLevel == level) {
        return;
    }

    if (currentLevel != NULL && currentLevel != level) {
        currentLevel->unload();
        printf("unloading level %p\n", currentLevel);
    }

    currentLevel = level;
    level->load();

    printf("loading level %p\n", level);

    if (music_current == NULL) {
        music_current = level->music;
        if (music_current != NULL)
            PlayMusicStream(*music_current);
    } else {
        if (music_current != level->music) {
            StopMusicStream(*music_current);

            music_current = level->music;
            if (music_current != NULL)
                PlayMusicStream(*music_current);
        } else if (!IsMusicStreamPlaying(*music_current)){
            PlayMusicStream(*music_current);
        }
    }
}

void reloadLevel() {
    if (currentLevel != NULL) {
        currentLevel->unload();
        currentLevel->load();
    }
}