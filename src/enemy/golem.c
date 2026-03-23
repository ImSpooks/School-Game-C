#include "enemy/golem.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "globals.h"
#include "items.h"
#include "enemy/enemy.h"
#include "vec.h"
#include "player/player.h"
#include "screen/screen.h"
#include "asset_manager.h"

void spawn_golem_projectile(Projectile **projectiles, int x, int y);
void golem_projectile_draw(Projectile *projectile);
Rectangle golem_projectile_hitbox(Projectile *projectile);

void enemy_golem_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_golem;

    enemy->music = vector_create();
    vector_add(&enemy->music, &assets.music_golem_intro);
    vector_add(&enemy->music, &assets.music_golem_loop);

    assets.music_golem_intro.looping = false;
}

void enemy_golem_unload(Enemy *enemy) {
    vector_free(enemy->music);
}

bool enemy_golem_attack(Projectile **projectiles, int type, float timer, int turn) {

    printf("%d\n", type);

    return true;
}

void enemy_golem_pre_defeat() {
    player.flags.boss_golem = true;
}

void enemy_golem_post_defeat() {

}

void spawn_golem_projectile(Projectile **projectiles, int x, int y) {
    // TODO

}

void golem_projectile_draw(Projectile *projectile) {
    // TODO
}

Rectangle golem_projectile_hitbox(Projectile *projectile) {
    return (Rectangle){projectile->position.x, projectile->position.y, projectile->texture->width, projectile->texture->height};
}