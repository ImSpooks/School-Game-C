#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"
#include "scene/scene_battle.h"
#include "player/player.h"
#include "enemy/type/wizard.h"

void wizard2_battle_wizard(struct Scene *scene, struct Hud *hud);
void wizard2_battle_wizard_callback(void* old_data, struct Scene *scene, struct Hud *hud);
void wizard2_go_upstairs(struct Scene *scene, struct Hud *hud);
void wizard2_go_back(struct Scene *scene, struct Hud *hud);

void level_wizard2_enter(struct Hud *hud) {
    level_registry.levelWizard2.texture = &assets.texture_level_wizard2;

    array_allocate(&hud->buttons, sizeof(struct Button), 2);

    if (player.flags.boss_wizard) {
        hud_set_dialogue(hud, "This looks like a neat place");

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Go upstairs",
            .on_click = wizard2_go_upstairs
        };
    } else {
        hud_set_dialogue_multi(hud, (char*[]) {
            "Wizard: Before you search everything in this house...",
            "... you need to battle me first!"
        }, 2);

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Attack wizard",
            .on_click = wizard2_battle_wizard
        };
    }

    ((struct Button*)hud->buttons.data)[1] = (struct Button) {
        .text = "Go back",
        .on_click = wizard2_go_back
    };

    hud->buttons.size = 2;
}

void level_wizard2_leave(struct Hud *hud) {

}

void wizard2_battle_wizard(struct Scene *scene, struct Hud *hud) {
    scene->request_scene_change.type = BATTLE;
    scene->request_scene_change.callback = wizard2_battle_wizard_callback;
}

void wizard2_battle_wizard_callback(void* old_data, struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    data->enemy = (Enemy) {
        .max_health = 2500,
        .health = 2500,
        .attack_stat = 16,
        .defence_stat = 10,
        .total_attacks = 4,

        .initialize = enemy_wizard_initialize,
        .unload = enemy_wizard_unload,
        .attack = enemy_wizard_attack,
        .pre_defeat = enemy_wizard_pre_defeat,
        .post_defeat = enemy_wizard_post_defeat
    };
}

void wizard2_go_upstairs(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelWizard3, hud);
}

void wizard2_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelWizard1, hud);
}