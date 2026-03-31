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

    void (*on_hit)(struct Projectile*);
};

void projectile_damage_player(struct Projectile* projectile);
void projectile_heal_player(struct Projectile* projectile);
void projectile_despawn(struct Projectile *projectile);
#endif //TESTRAYLIB_PROJECTILE_H