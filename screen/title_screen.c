#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../hud.h"
#include "screen.h"

void initializeTitleScreen();
void unloadTitleScreen();
void drawTitleScreen(const RenderTexture2D* texture);
void updateTitleScreen(const RenderTexture2D* texture);
void startButton();

Screen titleScreen = {
    .initialize = initializeTitleScreen,
    .unload = unloadTitleScreen,
    .update = updateTitleScreen,
    .draw = drawTitleScreen,
};

bool started = false;
float timer = 0;

static Button* buttons;

void initializeTitleScreen() {
    setDialogue("Click the button to start the game");

    buttons = (Button*) malloc(sizeof(Button) * 1);
    buttons[0] = (Button) {
        .text = "Start Game",
        .onClick = startButton,
    };

    setButtons(buttons, 1);
}

void unloadTitleScreen() {
    free(buttons);
}

void updateTitleScreen(const RenderTexture2D* texture) {
    if (started) {
        timer += GetFrameTime();
        if (timer > 1.0f) {
            //setScreen(&gameScreen);
        }
    }
}

void drawTitleScreen(const RenderTexture2D* texture) {
    DrawText("Click to start the game", 10, 10, 20, DARKGRAY);

    if (started) {
        const unsigned char alpha = (unsigned char) (timer > 1.0f ? 255 : (timer * 255));
        DrawRectangle(0, 0, GetScreenHeight(), GetScreenHeight(), (Color) {0, 0, 0, alpha});
    }
}

void startButton() {
    if (started) {
        return;
    }
    setDialogue("");
    setButtons(NULL, 0);
    started = true;
}