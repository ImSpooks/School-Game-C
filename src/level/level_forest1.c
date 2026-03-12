#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"

void loadLevelForest1();
void unloadLevelForest1();

void forest1_goLeft();
void forest1_goRight();
void forest1_goBack();


Level levelForest1 = {
    .load = loadLevelForest1,
    .unload = unloadLevelForest1
};

Button* forest1_buttons;

void loadLevelForest1() {
    levelForest1.texture = LoadTexture("assets/textures/levels/forest1.png");

    setDialogue("2 paths, which one should I take?");

    forest1_buttons = (Button*) malloc(sizeof(Button) * 3);

    forest1_buttons[0] = (Button) {
        .text = "Go left",
        .onClick = forest1_goLeft,
    };

    forest1_buttons[1] = (Button) {
        .text = "Go right",
        .onClick = forest1_goRight,
    };

    forest1_buttons[2] = (Button) {
        .text = "Go back",
        .onClick = forest1_goBack,
    };

    setButtons(forest1_buttons, 3);
}

void unloadLevelForest1() {
    free(forest1_buttons);
    UnloadTexture(levelForest1.texture);
}

void forest1_goLeft() {
    changeLevel(&levelVillage1);
}

void forest1_goRight() {
    changeLevel(&levelForest2);
}

void forest1_goBack() {
    changeLevel(&levelStart);
}