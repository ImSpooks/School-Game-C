#include "hawk.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "../../globals.h"
#include "../../items/items.h"
#include "enemy.h"
#include "vec.h"
#include "../../player/player.h"
#include "../../screen/screen.h"
#include "../../asset_manager.h"

void spawn_hawk_projectile(Projectile **projectiles, int x, int y);
void hawk_projectile_draw(Projectile *projectile);
Rectangle hawk_projectile_hitbox(Projectile *projectile);

void enemy_hawk_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_hawk;

    enemy->music = vector_create();
    vector_add(&enemy->music, &assets.music_hawk_intro);
    vector_add(&enemy->music, &assets.music_hawk_loop);

    assets.music_hawk_intro.looping = false;
}

void enemy_hawk_unload(Enemy *enemy) {
    vector_free(enemy->music);
}

bool enemy_hawk_attack(Projectile **projectiles, int type, float timer, int turn) {

    printf("%d\n", type);

    return true;
}

void enemy_hawk_pre_defeat() {
    player.flags.boss_hawk = true;
}

void enemy_hawk_post_defeat() {

}

void spawn_hawk_projectile(Projectile **projectiles, int x, int y) {
    // TODO

}

void hawk_projectile_draw(Projectile *projectile) {
    // TODO
}

Rectangle hawk_projectile_hitbox(Projectile *projectile) {
    return (Rectangle){projectile->position.x, projectile->position.y, projectile->texture->width, projectile->texture->height};
}