#ifndef TESTRAYLIB_ENEMY_H
#define TESTRAYLIB_ENEMY_H
#include <raylib.h>

#include "projectile/projectile.h"

typedef struct Enemy Enemy;

typedef void (*enemy_initialize_fn)(Enemy*);
typedef void (*enemy_unload_fn)(Enemy*);

struct Enemy {
    Texture2D texture;

    float max_health;
    float health;
    int attack_stat;
    int defence_stat;

    int total_attacks;

    enemy_initialize_fn initialize;
    enemy_unload_fn unload;
    bool (*attack)(Projectile* /*vector*/, int /*attack type*/, float /*timer*/, int /*turn*/); // returns true if attack is done
    void (*defeat)(void);

    Projectile* projectiles;
};



#endif //TESTRAYLIB_ENEMY_H