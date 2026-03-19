#ifndef TESTRAYLIB_BATTLE_H
#define TESTRAYLIB_BATTLE_H
#include "enemy/enemy.h"

#define CALCULATING_DAMAGE_LENGTH   1.5f
#define DEALING_DAMAGE_LENGTH       0.75f
#define RENDERING_DAMAGE_LENGTH     1.0f

#define BATTLE_BOUNDS (Rectangle) {(640 / 2) - (192 / 2), (360 / 3 * 2) - (132 / 2), 192, 132}
#define BATTLE_BOUNDS_MIDDLE (Vector2) {BATTLE_BOUNDS.x + BATTLE_BOUNDS.width / 2, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height / 2}
#define BATTLE_BOUNDS_EXPAND_TIME 0.15f

typedef enum BattleState {
    PLAYER_TURN,
    CALCULATING_DAMAGE,
    DEALING_DAMAGE,
    MOVE_HEALTHBAR,
    ENEMY,
    ENEMY_FINISH,
    FINISHED,
} BattleState;

typedef struct BattleOptions {
    BattleState state;
    float last_damage;

    int turn;
    float timer;
    int enemy_attack;
} BattleOptions;

typedef struct Battle {
    Enemy enemy;
    int turn;
} Battle;

extern Battle activeBattle;

void start_attack();
void strike_enemy(Enemy* enemy, int strength, bool ignore_attack);


#endif //TESTRAYLIB_BATTLE_H