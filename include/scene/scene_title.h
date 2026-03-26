#ifndef SCHOOLGAMEC_SCENE_TITLE_H
#define SCHOOLGAMEC_SCENE_TITLE_H

#include "scene/scene.h"

struct TitleData {
    bool started;
    float timer;
};

void title_scene_load(struct Scene *scene, struct Hud *hud);
void title_scene_unload(struct Scene *scene, struct Hud *hud);
void title_scene_update(struct Scene *scene, struct Hud *hud);
void title_scene_draw(void* scene_data);

#endif //SCHOOLGAMEC_SCENE_TITLE_H