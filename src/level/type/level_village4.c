#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"
#include "player/player.h"

void village4_sell_item(struct Scene *scene, struct Hud *hud);
void village4_go_back(struct Scene *scene, struct Hud *hud);

void level_village4_enter(struct Hud *hud) {
    level_registry.levelVillage4.texture = &assets.texture_level_village4;

    hud_set_dialogue(hud, "Merchant Andre: What can I do for you?");

    array_allocate(&hud->buttons, sizeof(struct Button), 2);

    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Sell an item",
        .on_click = village4_sell_item
    };

    ((struct Button*)hud->buttons.data)[1] = (struct Button) {
        .text = "Go back",
        .on_click = village4_go_back
    };

    hud->buttons.size = 2;
}

void level_village4_leave(struct Hud *hud) {

}

void village4_sell_item(struct Scene *scene, struct Hud *hud) {
    if (player_contains_item(player, SELLABLE)) {
        player_remove_item(&player, SELLABLE);

        player_add_item(&player, (struct Item) {
            .type = COIN,
            .texture = &assets.texture_item_coin,
            .value = 1
        });

        hud_set_dialogue(hud, "Merchant Andre: Thanks for selling me that item, here's a golden coin!");
    } else {
        hud_set_dialogue(hud, "You don't have any valuables to sell");
    }
}

void village4_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelVillage1, hud);
}
