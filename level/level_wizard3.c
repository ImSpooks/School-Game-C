#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"

void loadLevelWizard3();

void unloadLevelWizard3();

void wizard3_pickupPotion();

void wizard3_goBack();

Level levelWizard3 = {
    .load = loadLevelWizard3,
    .unload = unloadLevelWizard3
};

Button *wizard3_buttons;

void loadLevelWizard3() {
    levelWizard3.texture = LoadTexture("assets/levels/wizard3.png");

    setDialogue("This looks like a neat place");

    if (false) {
        // TODO potion inventory
        wizard3_buttons = (Button *) malloc(sizeof(Button) * 1);
        wizard3_buttons[0] = (Button){
            .text = "Go back",
            .onClick = wizard3_goBack
        };
        setButtons(wizard3_buttons, 1);
    } else {
        wizard3_buttons = (Button *) malloc(sizeof(Button) * 2);
        wizard3_buttons[0] = (Button){
            .text = "Pick up the potion",
            .onClick = wizard3_pickupPotion
        };

        wizard3_buttons[1] = (Button){
            .text = "Go back",
            .onClick = wizard3_goBack
        };
    setButtons(wizard3_buttons, 2);
    }

}

void unloadLevelWizard3() {
    free(wizard3_buttons);
    UnloadTexture(levelWizard3.texture);
}

void wizard3_pickupPotion() {
    setDialogue("Oh, a potion! Just what I needed! Lets go back to the Angel.");

    // TODO pickup potion
    reloadLevel();
}

void wizard3_goBack() {
    changeLevel(&levelWizard2);
}
