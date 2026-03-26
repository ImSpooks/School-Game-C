#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"
#include "scene/scene_battle.h"
#include "player/player.h"
#include "enemy/type/flowey.h"

void village3_battle_flowey(struct Scene *scene, struct Hud *hud);
void village3_battle_flowey_callback(void* old_data, struct Scene *scene, struct Hud *hud);
void village3_go_back(struct Scene *scene, struct Hud *hud);

void level_village3_enter(struct Hud *hud) {
    level_registry.levelVillage3.texture = &assets.texture_level_village3;

    array_allocate(&hud->buttons, sizeof(struct Button), 2);

    if (player.flags.boss_flowey) {
        hud_set_dialogue(hud, "Farmer Johan: Hey, how's it going?");

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Go back",
            .on_click = village3_go_back
        };

        hud->buttons.size = 1;
    } else {
        hud_set_dialogue_multi(hud, (char*[]) {
            "Farmer Johan: Could you help me defeat this monster?",
            "I'll give you a golden coin as a reward."
        }, 2);

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Attack the enemy",
            .on_click = village3_battle_flowey
        };

        ((struct Button*)hud->buttons.data)[1] = (struct Button) {
            .text = "Go back",
            .on_click = village3_go_back
        };

        hud->buttons.size = 2;
    }
}

void level_village3_leave(struct Hud *hud) {

}

void village3_battle_flowey(struct Scene *scene, struct Hud *hud) {
    scene->request_scene_change.type = BATTLE;
    scene->request_scene_change.callback = village3_battle_flowey_callback;
}

void village3_battle_flowey_callback(void* old_data, struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    data->enemy = (Enemy) {
        .max_health = 30,
        .health = 30,
        .attack_stat = 2,
        .defence_stat = 2,
        .total_attacks = 1,

        .initialize = enemy_flowey_initialize,
        .unload = enemy_flowey_unload,
        .attack = enemy_flowey_attack,
        .pre_defeat = enemy_flowey_pre_defeat,
        .post_defeat = enemy_flowey_post_defeat
    };
}

void village3_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelVillage1, hud);
}
