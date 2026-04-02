#include "scene/scene_credits.h"

void credits_scene_load(struct Scene *scene, struct Hud *hud) {
    hud_unset_flag(hud, RENDER_ITEMS);
    hud->buttons.size = 0;
}

void credits_scene_unload(struct Scene *scene, struct Hud *hud) {
}

void credits_scene_update(struct Scene *scene, struct Hud *hud) {
    struct CreditsData *data = scene->data;
    data->timer += GetFrameTime();
}

void credits_scene_draw(void *scene_data) {
}
