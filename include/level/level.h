#ifndef TESTRAYLIB_LEVEL_H
#define TESTRAYLIB_LEVEL_H

#include <raylib.h>
#include "hud.h"

struct Level {
    Texture2D* texture;
    Music* music;
    void (*enter)(struct Hud *hud);
    void (*leave)(struct Hud *hud);
};

#endif //TESTRAYLIB_LEVEL_H