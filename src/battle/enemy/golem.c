#include "golem.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "../../globals.h"
#include "../../items/items.h"
#include "enemy.h"
#include "vec.h"
#include "../../player/player.h"
#include "../../screen/screen.h"

Texture2D enemy_golem_projectile_texture;

void spawn_golem_projectile(Projectile **projectiles, int x, int y);
void golem_projectile_draw(Projectile *projectile);
Rectangle golem_projectile_hitbox(Projectile *projectile);

void enemy_golem_initialize(Enemy *enemy) {
    enemy->texture = LoadTexture("assets/textures/enemies/golem.png");

    enemy->music = LoadMusicStream("assets/music/boss/golem_intro.ogg");
    enemy->music.looping = false;
    enemy->music2 = LoadMusicStream("assets/music/boss/golem_main.ogg");
    enemy->music2.looping = true;

    enemy_golem_projectile_texture = LoadTexture("assets/textures/enemies/projectile/golem.png");
}

void enemy_golem_unload(Enemy *enemy) {
    UnloadTexture(enemy->texture);
    UnloadMusicStream(enemy->music);
    UnloadMusicStream(enemy->music2);

    UnloadTexture(enemy_golem_projectile_texture);
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