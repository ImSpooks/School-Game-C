#ifndef TESTRAYLIB_WIZARD_H
#define TESTRAYLIB_WIZARD_H
#include <stdbool.h>

#include "enemy.h"
#include "enemy/projectile/projectile.h"

void enemy_wizard_initialize(Enemy* enemy);
void enemy_wizard_unload(Enemy* enemy);
bool enemy_wizard_attack(Projectile** vector, int type, float timer, int turn);
void enemy_wizard_pre_defeat();
void enemy_wizard_post_defeat();

#endif //TESTRAYLIB_WIZARD_H