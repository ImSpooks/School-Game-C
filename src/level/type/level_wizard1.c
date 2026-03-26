#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"
#include "scene/scene_battle.h"
#include "player/player.h"
#include "enemy/type/golem.h"

void wizard1_battle_golem(struct Scene *scene, struct Hud *hud);
void wizard1_battle_golem_callback(void* old_data, struct Scene *scene, struct Hud *hud);
void wizard1_enter_house(struct Scene *scene, struct Hud *hud);
void wizard1_go_back(struct Scene *scene, struct Hud *hud);

void level_wizard1_enter(struct Hud *hud) {
    level_registry.levelWizard1.texture = &assets.texture_level_wizard1;

    array_allocate(&hud->buttons, sizeof(struct Button), 2);

    if (player.flags.boss_golem) {
        if (player.flags.boss_wizard) {
            hud_set_dialogue(hud, "It feels empty here...");
        } else {
            hud_set_dialogue(hud, "Wizard: So, I see you defeated my golem, well done.");
        }

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Enter house",
            .on_click = wizard1_enter_house
        };
    } else {
        hud_set_dialogue_multi(hud, (char*[]) {
            "???: I see, your adventure is almost over. But first,",
            "I'm gonna heal you, because...",
            "you must defeat my mighty GOLEM to continue on!"
        }, 3);

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Attack golem",
            .on_click = wizard1_battle_golem
        };
    }

    ((struct Button*)hud->buttons.data)[1] = (struct Button) {
        .text = "Go back",
        .on_click = wizard1_go_back
    };

    hud->buttons.size = 2;
}

void level_wizard1_leave(struct Hud *hud) {

}

void wizard1_battle_golem(struct Scene *scene, struct Hud *hud) {
    scene->request_scene_change.type = BATTLE;
    scene->request_scene_change.callback = wizard1_battle_golem_callback;
}

void wizard1_battle_golem_callback(void* old_data, struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    data->enemy = (Enemy) {
        .max_health = 1250,
        .health = 1250,
        .attack_stat = 10,
        .defence_stat = 14,
        .total_attacks = 3,

        .initialize = enemy_golem_initialize,
        .unload = enemy_golem_unload,
        .attack = enemy_golem_attack,
        .pre_defeat = enemy_golem_pre_defeat,
        .post_defeat = enemy_golem_post_defeat
    };

    player.health = player.maxHealth;
}

void wizard1_enter_house(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelWizard2, hud);
}

void wizard1_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelForest3, hud);
}