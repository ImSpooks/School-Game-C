#ifndef TESTRAYLIB_BATTLE_H
#define TESTRAYLIB_BATTLE_H
#include "enemy/enemy.h"

typedef enum BattleState {
    PLAYER_TURN,
    CALCULATING_DAMAGE,
    DEALING_DAMAGE,
    MOVE_HEALTHBAR,
    ENEMY
} BattleState;

typedef struct BattleOptions {
    BattleState state;
    float last_damage;

    int turn;
    float timer;
    int enemy_attack;
} BattleOptions;

typedef struct Battle {
    int invincibility_timer;
    Enemy enemy;
    int turn;
} Battle;

extern Battle activeBattle;

void start_attack();
void strike_enemy(Enemy* enemy, int strength, bool ignore_attack);


#endif //TESTRAYLIB_BATTLE_H