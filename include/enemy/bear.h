#ifndef TESTRAYLIB_BEAR_H
#define TESTRAYLIB_BEAR_H
#include <stdbool.h>

#include "enemy.h"
#include "enemy/projectile/projectile.h"

void enemy_bear_initialize(Enemy* enemy);
void enemy_bear_unload(Enemy* enemy);
bool enemy_bear_attack(Projectile** vector, int type, float timer, int turn);
void enemy_bear_pre_defeat();
void enemy_bear_post_defeat();

#endif //TESTRAYLIB_BEAR_H