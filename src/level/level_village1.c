#include <raylib.h>
#include <stdlib.h>

#include "level/level.h"
#include "hud.h"
#include "screen/game_screen.h"
#include "asset_manager.h"

void loadLevelVillage1();
void unloadLevelVillage1();

void village1_toBlacksmith();
void village1_toFarmer();
void village1_toMerchant();
void village1_goBack();

Level levelVillage1 = {
    .load = loadLevelVillage1,
    .unload = unloadLevelVillage1
};

Button* village1_buttons;

void loadLevelVillage1() {
    levelVillage1.texture = &assets.texture_level_village1;

    village1_buttons = (Button*) malloc(sizeof(Button) * 4);

    setDialogue("Where should I go first?");

    village1_buttons[0] = (Button) {
        .text = "To the Blacksmith",
        .onClick = village1_toBlacksmith
    };

    village1_buttons[1] = (Button) {
        .text = "Go the Farmer",
        .onClick = village1_toFarmer
    };

    village1_buttons[2] = (Button) {
        .text = "Go the Merchant",
        .onClick = village1_toMerchant
    };

    village1_buttons[3] = (Button) {
        .text = "Go back",
        .onClick = village1_goBack
    };

    setButtons(village1_buttons, 4);
}

void unloadLevelVillage1() {
    free(village1_buttons);
}

void village1_toBlacksmith() {
    changeLevel(&levelVillage2);
}

void village1_toFarmer() {
    changeLevel(&levelVillage3);
}

void village1_toMerchant() {
    changeLevel(&levelVillage4);
}

void village1_goBack() {
    changeLevel(&levelForest1);
}