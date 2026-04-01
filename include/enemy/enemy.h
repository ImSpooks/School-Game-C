#ifndef TESTRAYLIB_ENEMY_H
#define TESTRAYLIB_ENEMY_H

#include <raylib.h>
#include "hud.h"

typedef struct Enemy Enemy;

typedef void (*enemy_initialize_fn)(Enemy*);
typedef void (*enemy_unload_fn)(Enemy*);

struct Enemy {
    Texture2D* texture;
    struct Array music;

    float max_health;
    float health;
    int attack_stat;
    int defence_stat;

    int total_attacks;

    enemy_initialize_fn initialize;
    enemy_unload_fn unload;
    bool (*attack)(struct Array *projectiles, int rand_type, float timer, int turn, bool first_tick); // returns true if attack is done
    void (*pre_defeat)(void);
    void (*post_defeat)(struct Hud *hud);
};



#endif //TESTRAYLIB_ENEMY_H