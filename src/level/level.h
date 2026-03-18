#ifndef TESTRAYLIB_LEVEL_H
#define TESTRAYLIB_LEVEL_H

#include <raylib.h>

typedef struct Level {
    Texture2D texture;
    Music* music;
    void (*load)();
    void (*unload)();
} Level;

extern Level levelStart;

extern Level levelForest1;
extern Level levelForest2;
extern Level levelForest3;

extern Level levelVillage1;
extern Level levelVillage2;
extern Level levelVillage3;
extern Level levelVillage4;

extern Level levelWizard1;
extern Level levelWizard2;
extern Level levelWizard3;

#endif //TESTRAYLIB_LEVEL_H