#include "enemy/type/bear.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "globals.h"
#include "items.h"
#include "enemy/enemy.h"

#include "player/player.h"
#include "asset_manager.h"
#include "enemy/projectile/projectile.h"

void spawn_bear_projectile(struct Array *projectiles, int x, int y);
void bear_projectile_draw(struct Projectile *projectile);
Rectangle bear_projectile_hitbox(struct Projectile *projectile);

void enemy_bear_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_bear;

    if (enemy->music.capacity == 0) {
        array_allocate(&enemy->music, sizeof(Music*), 1);
        ((Music**) enemy->music.data)[0] = &assets.music_bear;
        enemy->music.size = 1;
    }
}

void enemy_bear_unload(Enemy *enemy) {

}

bool enemy_bear_attack(struct Array *projectiles, int rand_type, float timer, int turn) {

    printf("%d\n", rand_type);

    return true;
}

void enemy_bear_pre_defeat() {
    player.flags.boss_bear = true;

    player_add_item(&player, (struct Item) {
        .type = SELLABLE,
        .texture = &assets.texture_item_bear_tooth,
        .value = 1,
    });
}

void enemy_bear_post_defeat(struct Hud *hud) {
    hud_set_dialogue(hud, "You got one of his teeth as a reward. It might be worth something.");
}

void spawn_bear_projectile(struct Array *projectiles, int x, int y) {
    // TODO

}

void bear_projectile_draw(struct Projectile *projectile) {
    // TODO
}

Rectangle bear_projectile_hitbox(struct Projectile *projectile) {
    return (Rectangle){projectile->position.x, projectile->position.y, projectile->texture->width, projectile->texture->height};
}