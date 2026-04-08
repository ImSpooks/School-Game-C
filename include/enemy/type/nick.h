#ifndef TESTRAYLIB_NICK_H
#define TESTRAYLIB_NICK_H
#include <stdbool.h>

#include "enemy/enemy.h"
#include "hud.h"

void enemy_nick_initialize(Enemy* enemy);
void enemy_nick_unload(Enemy* enemy);
bool enemy_nick_attack(struct Array *projectiles, int rand_type, float timer, int turn, bool first_tick);
void enemy_nick_pre_defeat();
void enemy_nick_post_defeat(struct Hud *hud);

#endif //TESTRAYLIB_nick_H
