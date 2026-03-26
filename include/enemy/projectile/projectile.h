#ifndef TESTRAYLIB_PROJECTILE_H
#define TESTRAYLIB_PROJECTILE_H
#include <raylib.h>

struct Projectile;



struct Projectile {
    Vector2 position;
    Texture2D* texture;
    float damage;
    bool true_damage;
    float lifespan; // timer
    void* data;

    void (*draw)(struct Projectile*);
    Rectangle (*hitbox)(struct Projectile*);
};

bool projectile_collides_with_player(struct Projectile* projectile, Rectangle player_hitbox);
void projectile_damage_player(struct Projectile* projectile, bool ignore_armor);
#endif //TESTRAYLIB_PROJECTILE_H