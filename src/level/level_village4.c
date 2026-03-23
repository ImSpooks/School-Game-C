#include <raylib.h>
#include <stdlib.h>

#include "level/level.h"
#include "vec.h"
#include "hud.h"
#include "screen/game_screen.h"
#include "player/player.h"
#include "asset_manager.h"

void loadLevelVillage4();
void unloadLevelVillage4();

void village4_sellItem();
void village4_goBack();

Level levelVillage4 = {
    .load = loadLevelVillage4,
    .unload = unloadLevelVillage4
};

Button *village4_buttons;

void loadLevelVillage4() {
    levelVillage4.texture = &assets.texture_level_village4;

    setDialogue("Merchant Andre: What can I do for you?");

    village4_buttons = (Button *) malloc(sizeof(Button) * 2);

    village4_buttons[0] = (Button){
        .text = "Sell an item",
        .onClick = village4_sellItem
    };

    village4_buttons[1] = (Button){
        .text = "Go back",
        .onClick = village4_goBack
    };

    setButtons(village4_buttons, 2);
}

void unloadLevelVillage4() {
    free(village4_buttons);
}

void village4_sellItem() {
    if (contains_item(SELLABLE)) { // TODO check for any valuables
        remove_item_type(SELLABLE);

        Item* item = vector_add_dst(&player.inventory);
        item->type = COIN;
        item->texture = &assets.texture_item_coin;
        item->value = 1;

        setDialogue("Merchant Andre: Thanks for selling me that item, here's a golden coin!");
    } else {
        setDialogue("You don't have any valuables to sell");
    }
}

void village4_goBack() {
    changeLevel(&levelVillage1);
}
