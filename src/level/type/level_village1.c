#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"

void village1_to_blacksmith(struct Scene *scene, struct Hud *hud);
void village1_to_farmer(struct Scene *scene, struct Hud *hud);
void village1_to_merchant(struct Scene *scene, struct Hud *hud);
void village1_go_back(struct Scene *scene, struct Hud *hud);

void level_village1_enter(struct Hud *hud) {
    hud_set_dialogue(hud, "Where should I go first?");

    array_allocate(&hud->buttons, sizeof(struct Button), 4);

    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "To the Blacksmith",
        .on_click = village1_to_blacksmith
    };

    ((struct Button*)hud->buttons.data)[1] = (struct Button) {
        .text = "Go the Farmer",
        .on_click = village1_to_farmer
    };

    ((struct Button*)hud->buttons.data)[2] = (struct Button) {
        .text = "Go the Merchant",
        .on_click = village1_to_merchant
    };

    ((struct Button*)hud->buttons.data)[3] = (struct Button) {
        .text = "Go back",
        .on_click = village1_go_back
    };

    hud->buttons.size = 4;
}

void level_village1_leave(struct Hud *hud) {

}

void village1_to_blacksmith(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelVillage2, hud);
}

void village1_to_farmer(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelVillage3, hud);
}

void village1_to_merchant(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelVillage4, hud);
}

void village1_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelForest1, hud);
}