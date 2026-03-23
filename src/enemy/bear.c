#include "enemy/bear.h"

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

void spawn_bear_projectile(Projectile **projectiles, int x, int y);
void bear_projectile_draw(Projectile *projectile);
Rectangle bear_projectile_hitbox(Projectile *projectile);

void enemy_bear_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_bear;

    enemy->music = vector_create();
    vector_add(&enemy->music, &assets.music_bear);
}

void enemy_bear_unload(Enemy *enemy) {
    vector_free(enemy->music);
}

bool enemy_bear_attack(Projectile **projectiles, int type, float timer, int turn) {

    printf("%d\n", type);

    return true;
}

void enemy_bear_pre_defeat() {
    player.flags.boss_bear = true;

    Item* item = vector_add_dst(&player.inventory);
    item->type = SELLABLE;
    item->texture = &assets.texture_item_bear_tooth;
    item->value = 1;
}

void enemy_bear_post_defeat() {
    setDialogue("You got one of his teeth as a reward. It might be worth something.");
}

void spawn_bear_projectile(Projectile **projectiles, int x, int y) {
    // TODO

}

void bear_projectile_draw(Projectile *projectile) {
    // TODO
}

Rectangle bear_projectile_hitbox(Projectile *projectile) {
    return (Rectangle){projectile->position.x, projectile->position.y, projectile->texture->width, projectile->texture->height};
}