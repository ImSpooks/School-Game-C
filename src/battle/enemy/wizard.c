#include "wizard.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "../../globals.h"
#include "../../items/items.h"
#include "enemy.h"
#include "vec.h"
#include "../../player/player.h"
#include "../../asset_manager.h"

void spawn_wizard_projectile(Projectile **projectiles, int x, int y);
void wizard_projectile_draw(Projectile *projectile);
Rectangle wizard_projectile_hitbox(Projectile *projectile);

void enemy_wizard_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_wizard;

    enemy->music = vector_create();
    vector_add(&enemy->music, &assets.music_wizard);
}

void enemy_wizard_unload(Enemy *enemy) {
    vector_free(enemy->music);
}

bool enemy_wizard_attack(Projectile **projectiles, int type, float timer, int turn) {

    printf("%d\n", type);

    return true;
}

void enemy_wizard_pre_defeat() {
    player.flags.boss_wizard = true;
}

void enemy_wizard_post_defeat() {

}

void spawn_wizard_projectile(Projectile **projectiles, int x, int y) {
    // TODO

}

void wizard_projectile_draw(Projectile *projectile) {
    // TODO
}

Rectangle wizard_projectile_hitbox(Projectile *projectile) {
    return (Rectangle){projectile->position.x, projectile->position.y, projectile->texture->width, projectile->texture->height};
}