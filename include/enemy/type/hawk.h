#ifndef TESTRAYLIB_HAWK_H
#define TESTRAYLIB_HAWK_H
#include <stdbool.h>

#include "enemy/enemy.h"
#include "hud.h"

void enemy_hawk_initialize(Enemy* enemy);
void enemy_hawk_unload(Enemy* enemy);
bool enemy_hawk_attack(struct Array *projectiles, int rand_type, float timer, int turn);
void enemy_hawk_pre_defeat();
void enemy_hawk_post_defeat(struct Hud *hud);

#endif //TESTRAYLIB_HAWK_H