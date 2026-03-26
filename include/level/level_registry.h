#ifndef SCHOOLGAMEC_LEVEL_REGISTRY_H
#define SCHOOLGAMEC_LEVEL_REGISTRY_H

#include "level/level.h"

struct LevelRegistry {
    struct Level levelStart;

    struct Level levelForest1;
    struct Level levelForest2;
    struct Level levelForest3;

    struct Level levelVillage1;
    struct Level levelVillage2;
    struct Level levelVillage3;
    struct Level levelVillage4;

    struct Level levelWizard1;
    struct Level levelWizard2;
    struct Level levelWizard3;
};

extern struct LevelRegistry level_registry;

void level_registry_initialize(struct LevelRegistry* registry);

void level_start_enter(struct Hud *hud);
void level_start_leave(struct Hud *hud);

void level_forest1_enter(struct Hud *hud);
void level_forest1_leave(struct Hud *hud);

void level_forest2_enter(struct Hud *hud);
void level_forest2_leave(struct Hud *hud);

void level_forest3_enter(struct Hud *hud);
void level_forest3_leave(struct Hud *hud);

void level_wizard1_enter(struct Hud *hud);
void level_wizard1_leave(struct Hud *hud);

void level_wizard2_enter(struct Hud *hud);
void level_wizard2_leave(struct Hud *hud);

void level_wizard3_enter(struct Hud *hud);
void level_wizard3_leave(struct Hud *hud);

void level_village1_enter(struct Hud *hud);
void level_village1_leave(struct Hud *hud);

void level_village2_enter(struct Hud *hud);
void level_village2_leave(struct Hud *hud);

void level_village3_enter(struct Hud *hud);
void level_village3_leave(struct Hud *hud);

void level_village4_enter(struct Hud *hud);
void level_village4_leave(struct Hud *hud);

#endif //SCHOOLGAMEC_LEVEL_REGISTRY_H