#ifndef TESTRAYLIB_WIZARD_H
#define TESTRAYLIB_WIZARD_H
#include <stdbool.h>

#include "enemy/enemy.h"
#include "hud.h"

void enemy_wizard_initialize(Enemy* enemy);
void enemy_wizard_unload(Enemy* enemy);
bool enemy_wizard_attack(struct Array *projectiles, int rand_type, float timer, int turn, bool first_tick);
void enemy_wizard_pre_defeat();
void enemy_wizard_post_defeat(struct Hud *hud);

#endif //TESTRAYLIB_WIZARD_H