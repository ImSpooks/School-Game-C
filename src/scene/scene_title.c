#include "scene/scene_title.h"

#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "globals.h"
#include "enemy/enemy.h"
#include "enemy/type/nick.h"
#include "enemy/type/renoir.h"
#include "scene/scene_battle.h"
#include "scene/scene_level.h"

void start_button(struct Scene *scene, struct Hud *hud);

void fight_nick(struct Scene *scene, struct Hud *hud);
void fight_nick_callback(void* old_data, struct Scene *scene, struct Hud *hud);

void fight_renoir(struct Scene *scene, struct Hud *hud);
void fight_renoir_callback(void* old_data, struct Scene *scene, struct Hud *hud);




KeyboardKey *konami_code = (KeyboardKey[]) {
    KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT, KEY_B, KEY_A, KEY_ENTER
};
int konami_index = 0;

void title_scene_load(struct Scene *scene, struct Hud *hud) {
    hud_set_dialogue(hud, "Click the button to start the game");


    array_allocate(&hud->buttons, sizeof(struct Button), 3);
    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Start Game",
        .on_click = start_button,
    };

    ((struct Button*)hud->buttons.data)[1] = (struct Button) {
        .text = "Fight Nick",
        .on_click = fight_nick,
    };

    ((struct Button*)hud->buttons.data)[2] = (struct Button) {
        .text = "Fight Renoir",
        .on_click = fight_renoir,
    };

    hud->buttons.size = 1;
}

void title_scene_unload(struct Scene *scene, struct Hud *hud) {
    // no need
}

void title_scene_update(struct Scene *scene, struct Hud *hud) {
    struct TitleData *data = scene->data;

    if (data->started) {
        data->timer += GetFrameTime();
        if (data->timer > 1.0f) {

            scene->request_scene_change.type = LEVEL;
        }
    }

    KeyboardKey konami_current = konami_code[konami_index];
    if (IsKeyPressed(konami_current)) {
        konami_index++;
        if (konami_index >= 11) {
            konami_index = 0;

            //hud->buttons.size = 3;
        }
    }
}

void title_scene_draw(void* scene_data) {
    struct TitleData *data = scene_data;

    DrawText("Click to start the game", 10, 10, 20, DARKGRAY);

    if (data->started) {
        const unsigned char alpha = (unsigned char) (data->timer > 1.0f ? 255 : (data->timer * 255));
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {0, 0, 0, alpha});
    }
}

void start_button(struct Scene *scene, struct Hud *hud) {
    struct TitleData *data = scene->data;
    if (data->started) {
        return;
    }
    data->started = true;
    hud->buttons.size = 0;
}

void fight_nick(struct Scene *scene, struct Hud *hud) {
    scene->request_scene_change.type = BATTLE;
    scene->request_scene_change.callback = fight_nick_callback;
}

void fight_nick_callback(void* old_data, struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    data->enemy = (Enemy) {
        .max_health = 2500,
        .health = 2500,
        .attack_stat = 10,
        .defence_stat = 1,
        .total_attacks = 3,

        .initialize = enemy_nick_initialize,
        .unload = enemy_nick_unload,
        .attack = enemy_nick_attack,
        .pre_defeat = enemy_nick_pre_defeat,
        .post_defeat = enemy_nick_post_defeat
    };
}

void fight_renoir(struct Scene *scene, struct Hud *hud) {
    scene->request_scene_change.type = BATTLE;
    scene->request_scene_change.callback = fight_renoir_callback;
}

void fight_renoir_callback(void* old_data, struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    data->enemy = (Enemy) {
        .max_health = 2500,
        .health = 2500,
        .attack_stat = 10,
        .defence_stat = 1,
        .total_attacks = 3,

        .initialize = enemy_renoir_initialize,
        .unload = enemy_renoir_unload,
        .attack = enemy_renoir_attack,
        .pre_defeat = enemy_renoir_pre_defeat,
        .post_defeat = enemy_renoir_post_defeat
    };
}