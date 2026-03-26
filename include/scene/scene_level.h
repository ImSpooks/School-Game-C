#ifndef TESTRAYLIB_GAME_SCREEN_H
#define TESTRAYLIB_GAME_SCREEN_H

#include "scene/scene.h"
#include "level/level.h"

#define TRANSITION_LENGTH CLITERAL((float) 0.25)
#define TRANSITION_LENGTH_TOTAL CLITERAL((float) TRANSITION_LENGTH * 2.0)

struct SwappingLevelData {
    bool swapping;
    bool fadeIn;
    float timer;

    struct Level* new_level;
};

struct LevelData {
    struct Level* current_level;
    struct SwappingLevelData swapping_level_data;
};

void set_level(struct Scene *scene, const struct Level *level, struct Hud *hud);
void reload_level(const struct LevelData *level_data, struct Hud *hud);
void request_level_change(struct LevelData *level_data, struct Level *new_level);
void set_level_fade(struct Scene *scene, struct Level *new_level, struct Hud *hud);

void level_scene_load(struct Scene *scene, struct Hud *hud);
void level_scene_unload(struct Scene *scene, struct Hud *hud);
void level_scene_update(struct Scene *scene, struct Hud *hud);
void level_scene_draw(void* scene_data);

#endif //TESTRAYLIB_GAME_SCREEN_H