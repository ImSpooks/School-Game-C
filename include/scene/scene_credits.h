#ifndef SCHOOLGAMEC_SCENE_CREDITS_H
#define SCHOOLGAMEC_SCENE_CREDITS_H

#include "scene/scene.h"

struct CreditsData {
    float timer;
    float max_timer; // timer value at which "Thanks for playing!" is centered
};

void credits_scene_load(struct Scene *scene, struct Hud *hud);
void credits_scene_unload(struct Scene *scene, struct Hud *hud);
void credits_scene_update(struct Scene *scene, struct Hud *hud);
void credits_scene_draw(void* scene_data);

#endif //SCHOOLGAMEC_SCENE_CREDITS_H