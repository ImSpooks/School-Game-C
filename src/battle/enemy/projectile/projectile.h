#ifndef TESTRAYLIB_PROJECTILE_H
#define TESTRAYLIB_PROJECTILE_H
#include <raylib.h>

typedef struct Projectile Projectile;

struct Projectile {
    Vector2 position;
    Texture2D* texture;
    int damage;
    bool true_damage;
    float lifespan; // timer
    void* data;

    void (*draw)(Projectile*);
    Rectangle (*get_hitbox)(Projectile*);
};

void damage_player(Projectile* projectile, bool ignore_armor);
#endif //TESTRAYLIB_PROJECTILE_H