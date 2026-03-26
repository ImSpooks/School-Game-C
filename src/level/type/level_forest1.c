#include "level/level_registry.h"
#include "scene/scene_level.h"

void forest1_go_left(struct Scene *scene, struct Hud *hud);
void forest1_go_right(struct Scene *scene, struct Hud *hud);
void forest1_go_back(struct Scene *scene, struct Hud *hud);

void level_forest1_enter(struct Hud *hud) {
    hud_set_dialogue(hud, "2 paths, which one should I take?");

    array_allocate(&hud->buttons, sizeof(struct Button), 3);

    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Go left",
        .on_click = forest1_go_left,
    };

    ((struct Button*)hud->buttons.data)[1] = (struct Button) {
        .text = "Go right",
        .on_click = forest1_go_right,
    };

    ((struct Button*)hud->buttons.data)[2] = (struct Button) {
        .text = "Go back",
        .on_click = forest1_go_back,
    };
    hud->buttons.size = 3;
}

void level_forest1_leave(struct Hud *hud) {

}

void forest1_go_left(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelVillage1, hud);
}

void forest1_go_right(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelForest2, hud);
}

void forest1_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelStart, hud);
}