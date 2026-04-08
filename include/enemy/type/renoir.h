#ifndef TESTRAYLIB_RENOIR_H
#define TESTRAYLIB_RENOIR_H
#include <stdbool.h>

#include "enemy/enemy.h"
#include "hud.h"

void enemy_renoir_initialize(Enemy* enemy);
void enemy_renoir_unload(Enemy* enemy);
bool enemy_renoir_attack(struct Array *projectiles, int rand_type, float timer, int turn, bool first_tick);
void enemy_renoir_pre_defeat();
void enemy_renoir_post_defeat(struct Hud *hud);

#endif //TESTRAYLIB_RENOIR_H