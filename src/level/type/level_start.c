#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"
#include "scene/scene_battle.h"
#include "scene/scene.h"
#include "player/player.h"
#include "enemy/type/hawk.h"

void start_go_into_the_forest(struct Scene *scene, struct Hud *hud);
void start_give_health_potion(struct Scene *scene, struct Hud *hud);
void start_talk_to_angel(struct Scene *scene, struct Hud *hud);
void start_attack_hawk(struct Scene *scene, struct Hud *hud);
void start_attack_hawk_callback(void* old_data, struct Scene *scene, struct Hud *hud);

void level_start_enter(struct Hud *hud) {
    if (player.flags.boss_hawk) {
        // TODO: Game ending
        hud_set_dialogue(hud, "TODO: Game ending");
        hud->buttons.size = 0;
        return;
    }

    hud_set_dialogue(hud, "Your adventure started here.");

    array_allocate(&hud->buttons, sizeof(struct Button), 2);

    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Go into the woods",
        .on_click = start_go_into_the_forest
    };

    if (player_contains_item(player, POTION)) {
        ((struct Button*)hud->buttons.data)[1] = (struct Button) {
            .text = "Give health potion",
            .on_click = start_give_health_potion
        };
    } else {
        ((struct Button*)hud->buttons.data)[1] = (struct Button) {
            .text = "Talk to the Angel",
            .on_click = start_talk_to_angel
        };
    }

    hud->buttons.size = 2;
}

void level_start_leave(struct Hud *hud) {

}

void start_go_into_the_forest(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelForest1, hud);
}

void start_give_health_potion(struct Scene *scene, struct Hud *hud) {
    hud_set_dialogue(hud, "Angel: Thank you! Now help me defeat this monster!");

    player.flags.healed_angel = true;

    array_allocate(&hud->buttons, sizeof(struct Button), 1);

    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Attack the giant Hawk",
        .on_click = start_attack_hawk
    };

    hud->buttons.size = 1;
}

void start_talk_to_angel(struct Scene *scene, struct Hud *hud) {
    hud_set_dialogue(hud, "Angel: I got weakened by this giant hawk, please find a healing potion.");
}

void start_attack_hawk(struct Scene *scene, struct Hud *hud) {
    scene->request_scene_change.type = BATTLE;
    scene->request_scene_change.callback = start_attack_hawk_callback;
}

void start_attack_hawk_callback(void* old_data, struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    data->enemy = (Enemy) {
        .max_health = 3000,
        .health = 3000,
        .attack_stat = 18,
        .defence_stat = 15,
        .total_attacks = 4,

        .initialize = enemy_hawk_initialize,
        .unload = enemy_hawk_unload,
        .attack = enemy_hawk_attack,
        .pre_defeat = enemy_hawk_pre_defeat,
        .post_defeat = enemy_hawk_post_defeat
    };

    player.health = player.maxHealth;
}