#ifndef TESTRAYLIB_LEVEL_H
#define TESTRAYLIB_LEVEL_H

#include <raylib.h>

typedef struct Level {
    Texture2D texture;
    void (*load)();
    void (*unload)();
} Level;

extern Level levelStart;
extern Level levelForest1;

#endif //TESTRAYLIB_LEVEL_H