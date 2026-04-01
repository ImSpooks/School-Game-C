#ifndef SCHOOLGAMEC_SCENE_GAMEOVER_H
#define SCHOOLGAMEC_SCENE_GAMEOVER_H

#include "scene/scene.h"

struct GameOverData {
    float timer;
};

void gameover_scene_load(struct Scene *scene, struct Hud *hud);
void gameover_scene_unload(struct Scene *scene, struct Hud *hud);
void gameover_scene_update(struct Scene *scene, struct Hud *hud);
void gameover_scene_draw(void* scene_data);

#endif //SCHOOLGAMEC_SCENE_GAMEOVER_H