#include <raylib.h>
#include <stdlib.h>

#include "level/level.h"
#include "vec.h"
#include "hud.h"
#include "player/player.h"
#include "screen/game_screen.h"
#include "asset_manager.h"

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
    levelWizard3.texture = &assets.texture_level_wizard3;

    setDialogue("This looks like a neat place");

    if (contains_item(POTION)) {
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
}

void wizard3_pickupPotion() {
    setDialogue("Oh, a potion! Just what I needed! Lets go back to the Angel.");

    Item* cloak = vector_add_dst(&player.inventory);
    cloak->type = POTION;
    cloak->texture = &assets.texture_item_heal_potion;
    cloak->value = 1;

    reloadLevel();
}

void wizard3_goBack() {
    changeLevel(&levelWizard2);
}
