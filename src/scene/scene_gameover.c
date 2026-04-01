#include "scene/scene_gameover.h"

void gameover_scene_load(struct Scene *scene, struct Hud *hud) {
    scene->music->looping = false;

    if (!IsMusicStreamPlaying(*scene->music)) {
        PlayMusicStream(*scene->music);
    }

    struct GameOverData *data = scene->data;
    data->timer = 0.0f;
}

void gameover_scene_unload(struct Scene *scene, struct Hud *hud) {

}

void gameover_scene_update(struct Scene *scene, struct Hud *hud) {

}

void gameover_scene_draw(void* scene_data) {

}