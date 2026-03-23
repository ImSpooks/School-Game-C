#include <stdlib.h>
#include <raylib.h>
#include <stdio.h>
#include "vec.h"
#include "globals.h"

#include "screen/screen.h"
#include "hud.h"
#include "util/util.h"
#include "player/player.h"
#include "asset_manager.h"

int dialogueLines = 0;
bool dialogueAllocated = false;

Screen* screen = NULL;
struct HUDOptions hud;

void drawButtons();

int renderWidth = SCREEN_WIDTH * (1280 / SCREEN_WIDTH);
int renderHeight = SCREEN_HEIGHT * (720 / SCREEN_HEIGHT);

int frame = 0;
int main(void) {
    InitWindow(renderWidth, renderHeight, "Adventure Game");

    InitAudioDevice();

    assets_load();

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // The target's height is flipped (in the source Rectangle), due to OpenGL reasons

    setScreen(&titleScreen);

    SetTargetFPS(0);

    const RenderTexture2D screenRenderer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    hud.render_hud = true;
    hud.render_background = false;
    hud.render_buttons = true;
    hud.render_items = true;
    hud.render_health = false;

    player.inventory = vector_create();
    player.maxHealth = 100;
    player.health = player.maxHealth;

    while (!WindowShouldClose()) {
        renderWidth = GetScreenWidth();
        renderHeight = GetScreenHeight();

        if (screen == NULL) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            // just to be sure
            EndDrawing();
            continue;
        }
        // Update
        if (IsKeyDown(KEY_R) && GetScreenWidth() != 1280 && GetScreenHeight() != 720) {
                SetWindowSize(1280, 720);
        }
        if (IsKeyDown(KEY_O) && GetScreenWidth() != SCREEN_WIDTH && GetScreenHeight() != SCREEN_HEIGHT) {
            SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        }
        const float virtualWidthRatio = (float)renderWidth / (float)SCREEN_WIDTH;
        const float virtualHeightRatio = (float)renderHeight / (float)SCREEN_HEIGHT;

        screen->update(&screenRenderer);
        updateHud(&screenRenderer);

        // Draw
        // Screen
        BeginTextureMode(screenRenderer);
        {
            ClearBackground(RAYWHITE);
            screen->draw(&screenRenderer);

            drawHud(&screenRenderer);
        }
        EndTextureMode();

        BeginDrawing();


        DrawTexturePro(
            screenRenderer.texture,
            (Rectangle){0, 0, (float)screenRenderer.texture.width, -(float)screenRenderer.texture.height},
            (Rectangle){
                -virtualWidthRatio, -virtualHeightRatio, (float) renderWidth + virtualWidthRatio * 2.0f,
                (float) (renderHeight) + virtualHeightRatio * 2.0f
            },
            (Vector2){0, 0},
            0.0f,
            WHITE
        );

        EndDrawing();
        frame++;
    }
    screen->unload();

    UnloadRenderTexture(screenRenderer);

    assets_unload();

    if (dialogue.lines != NULL && dialogueAllocated) {
        free(dialogue.lines);
        dialogue.lines = NULL;
        dialogueAllocated = 0;
    }

    if (vector_size(player.inventory) > 0)
        vector_free(player.inventory);

    CloseWindow();

    return 0;
}

void setScreen(Screen* newScreen) {
    if (newScreen == screen) {
        return;
    }
    frame = 0;

    if (screen != NULL && screen != newScreen) {
        screen->unload();
    }

    screen = newScreen;
    newScreen->initialize();
}

void setDialogue(char* text) {
    setDialogueMulti((char*[]){ text }, 1);
}

void setDialogueMulti(char* text[], int lines) {
    if (dialogue.lines != NULL && dialogueAllocated) {
        free(dialogue.lines);
        dialogue.lines = NULL;
        dialogueAllocated = false;
    }

    if (lines <= 0) {
        dialogue.lines = NULL;
        dialogueLines = 0;
        dialogueAllocated = false;
        return;
    }

    char** newLines = malloc(sizeof(char*) * (size_t)lines);

    for (int i = 0; i < lines; i++) {
        newLines[i] = text[i];
    }

    dialogue.lines = newLines;
    dialogue.lineCount = lines;
    dialogueAllocated = true;
}

float getWidthScale() {
    const float virtualWidthRatio = (float)renderWidth / (float)SCREEN_WIDTH;
    return virtualWidthRatio;
}

float getHeightScale() {
    const float virtualHeightRatio = (float)renderHeight / (float)SCREEN_HEIGHT;
    return virtualHeightRatio;
}

Vector2 getScaledMousePos() {
    float mouseX = (float) GetMouseX() / getWidthScale();
    float mouseY = (float) GetMouseY() / getHeightScale();
    return (Vector2){mouseX, mouseY};
}