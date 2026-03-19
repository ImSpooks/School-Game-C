#include "bear.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "../../globals.h"
#include "../../items/items.h"
#include "enemy.h"
#include "vec.h"
#include "../../player/player.h"
#include "../../screen/screen.h"

Texture2D enemy_bear_projectile_texture;

void spawn_bear_projectile(Projectile **projectiles, int x, int y);
void bear_projectile_draw(Projectile *projectile);
Rectangle bear_projectile_hitbox(Projectile *projectile);

void enemy_bear_initialize(Enemy *enemy) {
    enemy->texture = LoadTexture("assets/textures/enemies/bear.png");
    enemy->music = LoadMusicStream("assets/music/boss/bear.ogg");
    enemy->music.looping = true;

    enemy_bear_projectile_texture = LoadTexture("assets/textures/enemies/projectile/bear.png");
}

void enemy_bear_unload(Enemy *enemy) {
    UnloadTexture(enemy->texture);
    UnloadMusicStream(enemy->music);

    UnloadTexture(enemy_bear_projectile_texture);
}

bool enemy_bear_attack(Projectile **projectiles, int type, float timer, int turn) {

    printf("%d\n", type);

    return true;
}

void enemy_bear_pre_defeat() {
    player.flags.boss_bear = true;

    Item* item = vector_add_dst(&player.inventory);
    item->type = SELLABLE;
    item->texture = &item_texture_bear_tooth;
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