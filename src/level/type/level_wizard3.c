#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"
#include "player/player.h"

void wizard3_pickup_potion(struct Scene *scene, struct Hud *hud);
void wizard3_go_back(struct Scene *scene, struct Hud *hud);

void level_wizard3_enter(struct Hud *hud) {
    level_registry.levelWizard3.texture = &assets.texture_level_wizard3;

    hud_set_dialogue(hud, "This looks like a neat place");

    if (player_contains_item(player, POTION)) {
        array_allocate(&hud->buttons, sizeof(struct Button), 1);

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Go back",
            .on_click = wizard3_go_back
        };

        hud->buttons.size = 1;
    } else {
        array_allocate(&hud->buttons, sizeof(struct Button), 2);

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Pick up the potion",
            .on_click = wizard3_pickup_potion
        };

        ((struct Button*)hud->buttons.data)[1] = (struct Button) {
            .text = "Go back",
            .on_click = wizard3_go_back
        };

        hud->buttons.size = 2;
    }
}

void level_wizard3_leave(struct Hud *hud) {

}

void wizard3_pickup_potion(struct Scene *scene, struct Hud *hud) {
    hud_set_dialogue(hud, "Oh, a potion! Just what I needed! Lets go back to the Angel.");

    player_add_item(&player, (struct Item) {
        .type = POTION,
        .texture = &assets.texture_item_heal_potion,
        .value = 1
    });

    reload_level(scene->data, hud);
}

void wizard3_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelWizard2, hud);
}
