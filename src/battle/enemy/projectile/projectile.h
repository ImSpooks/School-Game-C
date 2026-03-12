#ifndef TESTRAYLIB_PROJECTILE_H
#define TESTRAYLIB_PROJECTILE_H
#include <raylib.h>

typedef struct Projectile {
    Vector2 position;
    Texture2D* texture;
    int damage;
    float lifespan; // timer

    void (*draw)();
} Projectile;

void damage_player(Projectile* projectile, bool ignore_armor);
void collides_with_player(Projectile* projectile);

#endif //TESTRAYLIB_PROJECTILE_H