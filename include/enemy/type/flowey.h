#ifndef TESTRAYLIB_FLOWEY_H
#define TESTRAYLIB_FLOWEY_H
#include <stdbool.h>

#include "enemy/enemy.h"
#include "hud.h"

void enemy_flowey_initialize(Enemy* enemy);
void enemy_flowey_unload(Enemy* enemy);
bool enemy_flowey_attack(struct Array *projectiles, int rand_type, float timer, int turn, bool first_tick);
void enemy_flowey_pre_defeat();
void enemy_flowey_post_defeat(struct Hud *hud);

#endif //TESTRAYLIB_FLOWEY_H