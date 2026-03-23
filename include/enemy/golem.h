#ifndef TESTRAYLIB_GOLEM_H
#define TESTRAYLIB_GOLEM_H
#include <stdbool.h>

#include "enemy.h"
#include "enemy/projectile/projectile.h"

void enemy_golem_initialize(Enemy* enemy);
void enemy_golem_unload(Enemy* enemy);
bool enemy_golem_attack(Projectile** vector, int type, float timer, int turn);
void enemy_golem_pre_defeat();
void enemy_golem_post_defeat();

#endif //TESTRAYLIB_GOLEM_H