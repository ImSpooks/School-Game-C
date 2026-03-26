#include "enemy/type/golem.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "globals.h"
#include "items.h"
#include "enemy/enemy.h"

#include "player/player.h"
#include "scene/scene.h"
#include "asset_manager.h"
#include "enemy/projectile/projectile.h"

void spawn_golem_projectile(struct Projectile *projectiles, int x, int y);
void golem_projectile_draw(struct Projectile *projectile);
Rectangle golem_projectile_hitbox(struct Projectile *projectile);

void enemy_golem_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_golem;

    array_allocate(&enemy->music, sizeof(Music*), 2);
    ((Music**) enemy->music.data)[0] = &assets.music_golem_intro;
    ((Music**) enemy->music.data)[1] = &assets.music_golem_loop;

    assets.music_golem_intro.looping = false;
    enemy->music.size = 2;
}

void enemy_golem_unload(Enemy *enemy) {
    array_free(&enemy->music);
}

bool enemy_golem_attack(struct Array *projectiles, int rand_type, float timer, int turn) {

    printf("%d\n", rand_type);

    return true;
}

void enemy_golem_pre_defeat() {
    player.flags.boss_golem = true;
}

void enemy_golem_post_defeat(struct Hud *hud) {

}

void spawn_golem_projectile(struct Projectile *projectiles, int x, int y) {
    // TODO

}

void golem_projectile_draw(struct Projectile *projectile) {
    // TODO
}

Rectangle golem_projectile_hitbox(struct Projectile *projectile) {
    return (Rectangle){projectile->position.x, projectile->position.y, projectile->texture->width, projectile->texture->height};
}