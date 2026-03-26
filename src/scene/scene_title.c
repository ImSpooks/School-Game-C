#include "scene/scene_title.h"

#include <raylib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "globals.h"
#include "scene/scene_level.h"

void start_button(struct Scene *scene, struct Hud *hud);

void title_scene_load(struct Scene *scene, struct Hud *hud) {
    hud_set_dialogue(hud, "Click the button to start the game");


    array_allocate(&hud->buttons, sizeof(struct Button), 1);
    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Start Game",
        .on_click = start_button,
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