#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "vec.h"
#include "../hud.h"
#include "../screen/game_screen.h"
#include "../player/player.h"
#include "../asset_manager.h"

void loadLevelVillage2();
void unloadLevelVillage2();

void village2_buySword();
void village2_goBack();

Level levelVillage2 = {
    .load = loadLevelVillage2,
    .unload = unloadLevelVillage2
};

Button* village2_buttons;

void loadLevelVillage2() {
    levelVillage2.texture = &assets.texture_level_village2;

    setDialogue("Blacksmith Kukr: Hello there, how are ya doin'?");

    village2_buttons = (Button*) malloc(sizeof(Button) * 2);

    village2_buttons[0] = (Button) {
        .text = "Buy a sword",
        .onClick = village2_buySword
    };

    village2_buttons[1] = (Button) {
        .text = "Go back",
        .onClick = village2_goBack
    };

    setButtons(village2_buttons, 2);
}

void unloadLevelVillage2() {
    free(village2_buttons);
}

void village2_buySword() {
    if (contains_item(COIN)) {
        setDialogue("Blacksmith: Pleasure doing business.");

        remove_item_type(COIN);
        Item* item = vector_add_dst(&player.inventory);
        item->type = ATTACK;
        item->texture = &assets.texture_item_sword;
        item->value = 10;

    } else {
        setDialogueMulti((char*[]) {"Blacksmith Kukr: You don't have a golden coin to buy a sword.", "I've heard farmer Johan needs help and may reward you."}, 2);
    }
}

void village2_goBack() {
    changeLevel(&levelVillage1);
}