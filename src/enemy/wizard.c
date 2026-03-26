#include "enemy/type/wizard.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "globals.h"
#include "items.h"
#include "enemy/enemy.h"

#include "player/player.h"
#include "asset_manager.h"
#include "enemy/projectile/projectile.h"

void spawn_wizard_projectile(struct Projectile *projectiles, int x, int y);
void wizard_projectile_draw(struct Projectile *projectile);
Rectangle wizard_projectile_hitbox(struct Projectile *projectile);

void enemy_wizard_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_wizard;

    array_allocate(&enemy->music, sizeof(Music*), 1);
    ((Music**) enemy->music.data)[0] = &assets.music_wizard;
    enemy->music.size = 1;
}

void enemy_wizard_unload(Enemy *enemy) {
    array_free(&enemy->music);
}

bool enemy_wizard_attack(struct Array *projectiles, int rand_type, float timer, int turn) {

    printf("%d\n", rand_type);

    return true;
}

void enemy_wizard_pre_defeat() {
    player.flags.boss_wizard = true;
}

void enemy_wizard_post_defeat(struct Hud *hud) {

}

void spawn_wizard_projectile(struct Projectile *projectiles, int x, int y) {
    // TODO

}

void wizard_projectile_draw(struct Projectile *projectile) {
    // TODO
}

Rectangle wizard_projectile_hitbox(struct Projectile *projectile) {
    return (Rectangle){projectile->position.x, projectile->position.y, projectile->texture->width, projectile->texture->height};
}