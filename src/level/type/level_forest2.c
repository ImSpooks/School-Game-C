#include <raylib.h>
#include <stdlib.h>

#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"
#include "player/player.h"
#include "enemy/type/bear.h"
#include "scene/scene_battle.h"

void forest2_attack_bear(struct Scene *scene, struct Hud *hud);
void forest2_attack_bear_callback(void* old_data, struct Scene *scene, struct Hud *hud);
void forest2_go_forward(struct Scene *scene, struct Hud *hud);
void forest2_go_back(struct Scene *scene, struct Hud *hud);

void level_forest2_enter(struct Hud *hud) {
    if (player.flags.boss_bear)
        level_registry.levelForest2.texture = &assets.texture_level_forest2_nobear;
    else
        level_registry.levelForest2.texture = &assets.texture_level_forest2_bear;

    array_allocate(&hud->buttons, sizeof(struct Button), 2);

    if (player.flags.boss_bear) {
        hud_set_dialogue_multi(hud, (char*[]) {"Voiceover Pete: Alright, the bear is gone.", "You may now proceed safely."}, 2);

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Go forward",
            .on_click = forest2_go_forward
        };

        ((struct Button*)hud->buttons.data)[1] = (struct Button) {
            .text = "Go back",
            .on_click = forest2_go_back
        };

        hud->buttons.size = 2;

    } else {
        int button = 0;

        hud_set_dialogue_multi(
            hud,
            (char*[]){
                "Voiceover Pete: Attention all humans, this bear is really dangerous.",
                "You gotta help us out!",
                "The only thing you need to do is to fight the bear",
                "with a sword!",
                "",
                player_contains_item(player, ATTACK)
                    ? "You have a sword with you, what are you waiting for?!"
                    : "You dont have a sword? You can get one at the village."
            },
            6
        );


        if (player_contains_item(player, ATTACK)) {
            ((struct Button*)hud->buttons.data)[0] = (struct Button) {
                .text = "Attack the bear",
                .on_click = forest2_attack_bear
            };
            button++;
        }

        ((struct Button*)hud->buttons.data)[button] = (struct Button) {
            .text = "Go back",
            .on_click = forest2_go_back
        };
        hud->buttons.size = button+1;
    }
}

void level_forest2_leave(struct Hud *hud) {

}

void forest2_attack_bear(struct Scene *scene, struct Hud *hud) {
    scene->request_scene_change.type = BATTLE;
    scene->request_scene_change.callback = forest2_attack_bear_callback;
}

void forest2_attack_bear_callback(void* old_data, struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    data->enemy = (Enemy) {
        .max_health = 250,
        .health = 250,
        .attack_stat = 2,
        .defence_stat = 8,
        .total_attacks = 3,

        .initialize = enemy_bear_initialize,
        .unload = enemy_bear_unload,
        .attack = enemy_bear_attack,
        .pre_defeat = enemy_bear_pre_defeat,
        .post_defeat = enemy_bear_post_defeat
    };
}

void forest2_go_forward(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelForest3, hud);
}

void forest2_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelForest1, hud);
}