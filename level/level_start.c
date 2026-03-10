#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"
#include "../screen/screen.h"

void loadLevelStart();
void unloadLevelStart();

void goIntoTheForest();
void giveHealthPotion();
void talkToAngel();
void attackHawk();

Level levelStart = {
    .load = loadLevelStart,
    .unload = unloadLevelStart,
};

static Button* buttons;

static bool firstLoaded = false;

void loadLevelStart() {
    levelStart.texture = LoadTexture("assets/levels/start.png");

    if (firstLoaded) {
        firstLoaded = true;
        setDialogue("You woke up, in this strange forest.");
    } else {
        setDialogue("Your adventure started here.");
    }


    buttons = (Button*) malloc(sizeof(Button) * 2);

    buttons[0] = (Button) {
        .text = "Go into the woods",
        .onClick = goIntoTheForest,
    };

    if (false) { // TODO: Check for healing potion
        buttons[1] = (Button) {
            .text = "Give health potion",
            .onClick = giveHealthPotion
        };
    } else {
        buttons[1] = (Button) {
            .text = "Talk to the Angel",
            .onClick = talkToAngel,
        };
    }


    setButtons(buttons, 2);
}

void unloadLevelStart() {
    free(buttons);
    UnloadTexture(levelStart.texture);
}

void goIntoTheForest() {
    changeLevel(&levelForest1);
}

void giveHealthPotion() {
    setDialogue("Angel: Thank you! Now help me defeat this monster!");

    buttons = (Button*) malloc(sizeof(Button) * 1);

    buttons[0] = (Button) {
        .text = "Attack the giant Hawk",
        .onClick = attackHawk,
    };

    setButtons(buttons, 1);
}

void talkToAngel() {
    setDialogue("Angel: I got weakened by this giant hawk, please find a healing potion.");
}

void attackHawk() {
    
}