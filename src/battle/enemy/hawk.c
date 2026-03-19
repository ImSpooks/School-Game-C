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

Texture2D enemy_hawk_projectile_texture;

void spawn_hawk_projectile(Projectile **projectiles, int x, int y);
void hawk_projectile_draw(Projectile *projectile);
Rectangle hawk_projectile_hitbox(Projectile *projectile);

void enemy_hawk_initialize(Enemy *enemy) {
    enemy->texture = LoadTexture("assets/textures/enemies/hawk.png");

    enemy->music = LoadMusicStream("assets/music/boss/hawk_intro.ogg");
    enemy->music.looping = false;
    enemy->music2 = LoadMusicStream("assets/music/boss/hawk_main.ogg");
    enemy->music2.looping = true;

    enemy_hawk_projectile_texture = LoadTexture("assets/textures/enemies/projectile/hawk.png");
}

void enemy_hawk_unload(Enemy *enemy) {
    UnloadTexture(enemy->texture);
    UnloadMusicStream(enemy->music);
    UnloadMusicStream(enemy->music2);

    UnloadTexture(enemy_hawk_projectile_texture);
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