#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"
#include "../screen/screen.h"

void loadLevelForest1();
void unloadLevelForest1();

void goLeft();
void goRight();
void goBack();


Level levelForest1 = {
    .load = loadLevelForest1,
    .unload = unloadLevelForest1
};

static Button* buttons;

void loadLevelForest1() {
    levelForest1.texture = LoadTexture("assets/levels/forest_1.png");

    buttons = (Button*) malloc(sizeof(Button) * 3);

    buttons[0] = (Button) {
        .text = "Go left",
        .onClick = goLeft,
    };

    buttons[1] = (Button) {
        .text = "Go right",
        .onClick = goRight,
    };

    buttons[2] = (Button) {
        .text = "Go back",
        .onClick = goBack,
    };

    setButtons(buttons, 3);
}

void unloadLevelForest1() {
    free(buttons);
    UnloadTexture(levelForest1.texture);
}

void goLeft() {

}

void goRight() {

}

void goBack() {
    changeLevel(&levelStart);
}