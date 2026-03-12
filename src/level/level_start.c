#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"
#include "../screen/screen.h"

void loadLevelStart();
void unloadLevelStart();

void start_goIntoTheForest();
void start_giveHealthPotion();
void start_talkToAngel();
void start_attackHawk();

Level levelStart = {
    .load = loadLevelStart,
    .unload = unloadLevelStart,
};

Button* start_buttons;

bool firstLoaded = false;

void loadLevelStart() {
    levelStart.texture = LoadTexture("assets/textures/levels/start.png");

    if (firstLoaded) {
        firstLoaded = true;
        setDialogue("You woke up, in this strange forest.");
    } else {
        setDialogue("Your adventure started here.");
    }


    start_buttons = (Button*) malloc(sizeof(Button) * 2);

    start_buttons[0] = (Button) {
        .text = "Go into the woods",
        .onClick = start_goIntoTheForest,
    };

    if (false) { // TODO: Check for healing potion
        start_buttons[1] = (Button) {
            .text = "Give health potion",
            .onClick = start_giveHealthPotion
        };
    } else {
        start_buttons[1] = (Button) {
            .text = "Talk to the Angel",
            .onClick = start_talkToAngel,
        };
    }


    setButtons(start_buttons, 2);
}

void unloadLevelStart() {
    free(start_buttons);
    UnloadTexture(levelStart.texture);
}

void start_goIntoTheForest() {
    changeLevel(&levelForest1);
}

void start_giveHealthPotion() {
    setDialogue("Angel: Thank you! Now help me defeat this monster!");

    start_buttons = (Button*) malloc(sizeof(Button) * 1);

    start_buttons[0] = (Button) {
        .text = "Attack the giant Hawk",
        .onClick = start_attackHawk,
    };

    setButtons(start_buttons, 1);
}

void start_talkToAngel() {
    setDialogue("Angel: I got weakened by this giant hawk, please find a healing potion.");
}

void start_attackHawk() {
    
}