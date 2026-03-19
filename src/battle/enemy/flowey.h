#ifndef TESTRAYLIB_FLOWEY_H
#define TESTRAYLIB_FLOWEY_H
#include <stdbool.h>

#include "enemy.h"
#include "projectile/projectile.h"

void enemy_flowey_initialize(Enemy* enemy);
void enemy_flowey_unload(Enemy* enemy);
bool enemy_flowey_attack(Projectile** vector, int type, float timer, int turn);
void enemy_flowey_pre_defeat();
void enemy_flowey_post_defeat();

#endif //TESTRAYLIB_FLOWEY_H