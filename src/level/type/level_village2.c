#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"
#include "player/player.h"

void village2_buy_sword(struct Scene *scene, struct Hud *hud);
void village2_go_back(struct Scene *scene, struct Hud *hud);

void level_village2_enter(struct Hud *hud) {
    hud_set_dialogue(hud, "Blacksmith Kukr: Hello there, how are ya doin'?");

    array_allocate(&hud->buttons, sizeof(struct Button), 2);

    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Buy a sword",
        .on_click = village2_buy_sword
    };

    ((struct Button*)hud->buttons.data)[1] = (struct Button) {
        .text = "Go back",
        .on_click = village2_go_back
    };

    hud->buttons.size = 2;
}

void level_village2_leave(struct Hud *hud) {

}

void village2_buy_sword(struct Scene *scene, struct Hud *hud) {
    if (player_contains_item(player, COIN)) {
        hud_set_dialogue(hud, "Blacksmith: Pleasure doing business.");

        player_remove_item(&player, COIN);

        player_add_item(&player, (struct Item) {
            .type = ATTACK,
            .texture = &assets.texture_item_sword,
            .value = 10
        });
    } else {
        hud_set_dialogue_multi(hud, (char*[]) {
            "Blacksmith Kukr: You don't have a golden coin to buy a sword.",
            "I've heard farmer Johan needs help and may reward you."
        }, 2);
    }
}

void village2_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelVillage1, hud);
}