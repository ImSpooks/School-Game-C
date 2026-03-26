#include "scene/scene_level.h"

#include <stddef.h>

#include "globals.h"
#include "hud.h"
#include "asset_manager.h"


void level_scene_load(struct Scene *scene, struct Hud *hud) {
    const struct LevelData *data = scene->data;

    data->current_level->enter(hud);
    if (IsMusicValid(*data->current_level->music) && !IsMusicStreamPlaying(*data->current_level->music)) {
        PlayMusicStream(*data->current_level->music);
    }
    scene->music = data->current_level->music;
}

void level_scene_unload(struct Scene *scene, struct Hud *hud) {
    const struct LevelData *data = scene->data;

    data->current_level->leave(hud);
    if (IsMusicValid(*data->current_level->music) && IsMusicStreamPlaying(*data->current_level->music)) {
        StopMusicStream(*data->current_level->music);
    }
}

void level_scene_draw(void* scene_data) {
    const struct LevelData *data = scene_data;

    const struct Level *current_level = data->current_level;

    DrawTexturePro(*current_level->texture,
        (Rectangle) {0, 0, (float) (*current_level->texture).width, (float) (*current_level->texture).height},
        (Rectangle) {0, 0, (float) SCREEN_WIDTH, (float) SCREEN_HEIGHT},
        (Vector2) {0, 0},
        0.0f,
        WHITE);

    if (data->swapping_level_data.swapping) {
        int alpha;
        if (data->swapping_level_data.fadeIn) {

            alpha = (int) (data->swapping_level_data.timer / TRANSITION_LENGTH * 255);
        } else {
            alpha = 255 - (int) ((data->swapping_level_data.timer - TRANSITION_LENGTH) / TRANSITION_LENGTH * 255);
        }

        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {0, 0, 0, (unsigned char) alpha});
    }
}

void level_scene_update(struct Scene *scene, struct Hud *hud) {
    struct LevelData *data = scene->data;

    if (data->swapping_level_data.swapping) {
        data->swapping_level_data.timer += GetFrameTime();

        if (data->swapping_level_data.timer > TRANSITION_LENGTH_TOTAL) {
            data->swapping_level_data.swapping = false;
            hud_set_flag(hud, RENDER_BUTTONS);
        }
        if (data->swapping_level_data.timer > TRANSITION_LENGTH && data->swapping_level_data.fadeIn) {
            data->swapping_level_data.fadeIn = false;

            set_level(scene, data->swapping_level_data.new_level, hud);
        }
    }
}

void set_level(struct Scene *scene, const struct Level *level, struct Hud *hud) {
    struct LevelData *data = scene->data;


    data->current_level->leave(hud);

    if (data->current_level->music != level->music) { // change music
        if (IsMusicValid(*data->current_level->music) && IsMusicStreamPlaying(*data->current_level->music)) {
            StopMusicStream(*data->current_level->music);
        }

        if (level->music != NULL && IsMusicValid(*level->music) && !IsMusicStreamPlaying(*level->music)) {
            PlayMusicStream(*level->music);
            scene->music = level->music;
        }
    }

    level->enter(hud);
    data->current_level = (struct Level*) level;
}

void reload_level(const struct LevelData *level_data, struct Hud *hud) {
    level_data->current_level->leave(hud);
    level_data->current_level->enter(hud);
}

void set_level_fade(struct Scene *scene, struct Level *level, struct Hud *hud) {
    if (TRANSITION_LENGTH == 0) {
        set_level(scene, level, hud);
        return;
    }
    struct LevelData *data = scene->data;

    if (data->swapping_level_data.swapping) {
        return;
    }

    hud_unset_flag(hud, RENDER_BUTTONS);
    data->swapping_level_data.timer = 0;
    data->swapping_level_data.new_level = level;
    data->swapping_level_data.fadeIn = true;
    data->swapping_level_data.swapping = true;
}