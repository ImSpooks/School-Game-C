#include <stdlib.h>
#include <raylib.h>
#include <stdio.h>
#include "vec.h"

#include "screen/screen.h"
#include "hud.h"
#include "../../../../../Program Files (x86)/Windows Kits/10/include/10.0.26100.0/ucrt/stdlib.h"
#include "util/util.h"
#include "player/player.h"

char** dialogue = NULL;
int dialogueLines = 0;
bool dialogueAllocated = false;

Screen* screen = NULL;
HUD hud;

void drawButtons();

const int screenWidth = 320 * 2;
const int screenHeight = 180 * 2;

int renderWidth = screenWidth * (1280 / screenWidth);
int renderHeight = screenHeight * (720 / screenHeight);

int main(void) {
    InitWindow(renderWidth, renderHeight, "Adventure Game");

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // The target's height is flipped (in the source Rectangle), due to OpenGL reasons

    setScreen(&titleScreen);

    SetTargetFPS(0);

    const RenderTexture2D screenRenderer = LoadRenderTexture(screenWidth, screenHeight);
    const RenderTexture2D hudRenderer = LoadRenderTexture(screenWidth, screenHeight / 4);

    hud.texture = hudRenderer;
    hud.offset = (Rectangle){0, (float)screenHeight / 4 * 3, (float)hudRenderer.texture.width, (float)hudRenderer.texture.height};

    hud.options.render_hud = true;
    hud.options.render_background = false;
    hud.options.render_buttons = true;

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
        updateHud(&hudRenderer);

        // Draw
        // Screen
        BeginTextureMode(screenRenderer);
        {
            ClearBackground(RAYWHITE);
            screen->draw(&screenRenderer);

            if (dialogue != NULL) {
                for (int i = 0; i < dialogueLines; i++) {
                    const char* text = *(dialogue + i);
                    int width = MeasureText(text, 8);
                    DrawOutlinedText(text, (screenRenderer.texture.width / 2) - (width / 2), (int)(hud.offset.y - 22) - ((dialogueLines - i) * 15), 12, WHITE, 1, BLACK);
                }
            }
        }
        EndTextureMode();

        // Hud
        BeginTextureMode(hudRenderer);
        {
            if (hud.options.render_background) {
                ClearBackground((Color){ 80, 80, 80, (int) (255.0 / 4.0) });
            } else {
                ClearBackground((Color){ 0, 0, 0, 0 });
            }
            drawHud(&hudRenderer);
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


        DrawTexturePro(
            hudRenderer.texture,
            (Rectangle){0, 0, (float) hudRenderer.texture.width, -(float) hudRenderer.texture.height},
            (Rectangle){
                -virtualWidthRatio, -virtualHeightRatio,
                (float) renderWidth + virtualWidthRatio * 2.0f, (float) renderHeight / 4.0f + virtualHeightRatio * 2.0f
            },
            (Vector2){0, (float) -(renderHeight / 4.0 * 3.0)},
            0.0f,
            WHITE
        );

        EndDrawing();
    }
    UnloadRenderTexture(screenRenderer);
    UnloadRenderTexture(hudRenderer);

    if (dialogue != NULL && dialogueAllocated) {
        free(dialogue);
        dialogue = NULL;
        dialogueAllocated = 0;
    }

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
    if (dialogue != NULL && dialogueAllocated) {
        free(dialogue);
        dialogue = NULL;
        dialogueAllocated = false;
    }

    if (lines <= 0) {
        dialogue = NULL;
        dialogueLines = 0;
        dialogueAllocated = false;
        return;
    }

    char** newLines = malloc(sizeof(char*) * (size_t)lines);

    for (int i = 0; i < lines; i++) {
        newLines[i] = text[i];
    }

    dialogue = newLines;
    dialogueLines = lines;
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

Vector2 getScaledMousePosOffset(Vector2 offset) {
    float mouseX = (float) GetMouseX() / getWidthScale();
    float mouseY = (float) GetMouseY() / getHeightScale();
    return (Vector2) { -offset.x + mouseX, -offset.y + mouseY };
}