#ifndef TESTRAYLIB_SCENE_H
#define TESTRAYLIB_SCENE_H
#include <raylib.h>
#include "hud.h"

struct Scene;

enum SceneType {
    NONE,
    TITLE,
    LEVEL,
    BATTLE,
    GAME_OVER,
    CREDITS,
};

struct SceneChange {
    enum SceneType type;
    void (*callback)(void* old_data, struct Scene* new_scene, struct Hud *hud);
};

struct Scene {
    void (*load)(struct Scene *scene, struct Hud *hud);
    void (*unload)(struct Scene *scene, struct Hud *hud);
    void (*post_unload)(struct Scene *scene, struct Hud *hud);
    void (*update)(struct Scene *scene, struct Hud *hud);
    void (*draw)(void* scene_data);

    Music* music;
    struct Hud* hud;
    void* data;

    struct SceneChange request_scene_change;
};

#endif //TESTRAYLIB_SCENE_H