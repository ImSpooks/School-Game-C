#include <stdlib.h>
#include <raylib.h>
#include <stdio.h>
#include "vec.h"

#include "screen/screen.h"
#include "hud.h"
#include "../../../../../Program Files (x86)/Windows Kits/10/include/10.0.26100.0/ucrt/stdlib.h"
#include "util/util.h"
#include "player/player.h"

int dialogueLines = 0;
bool dialogueAllocated = false;

Screen* screen = NULL;
struct HUDOptions hud;

void drawButtons();

const int screenWidth = 320 * 2;
const int screenHeight = 180 * 2;

int renderWidth = screenWidth * (1280 / screenWidth);
int renderHeight = screenHeight * (720 / screenHeight);

int main(void) {
    InitWindow(renderWidth, renderHeight, "Adventure Game");

    InitAudioDevice();

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // The target's height is flipped (in the source Rectangle), due to OpenGL reasons

    setScreen(&titleScreen);

    SetTargetFPS(0);

    const RenderTexture2D screenRenderer = LoadRenderTexture(screenWidth, screenHeight);

    hud.render_hud = true;
    hud.render_background = false;
    hud.render_buttons = true;

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
        if (IsKeyDown(KEY_O) && GetScreenWidth() != screenWidth && GetScreenHeight() != screenHeight) {
            SetWindowSize(screenWidth, screenHeight);
        }
        const float virtualWidthRatio = (float)renderWidth / (float)screenWidth;
        const float virtualHeightRatio = (float)renderHeight / (float)screenHeight;

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
    }
    UnloadRenderTexture(screenRenderer);

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

    const Screen* prevScreen = screen;

    screen = newScreen;
    newScreen->initialize();

    if (prevScreen != NULL && prevScreen != newScreen) {
        prevScreen->unload();
    }
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
    const float virtualWidthRatio = (float)renderWidth / (float)screenWidth;
    return virtualWidthRatio;
}

float getHeightScale() {
    const float virtualHeightRatio = (float)renderHeight / (float)screenHeight;
    return virtualHeightRatio;
}

Vector2 getScaledMousePos() {
    float mouseX = (float) GetMouseX() / getWidthScale();
    float mouseY = (float) GetMouseY() / getHeightScale();
    return (Vector2){mouseX, mouseY};
}