#include "wizard.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "../../globals.h"
#include "../../items/items.h"
#include "enemy.h"
#include "vec.h"
#include "../../player/player.h"
#include "../../screen/screen.h"

Texture2D enemy_wizard_projectile_texture;

void spawn_wizard_projectile(Projectile **projectiles, int x, int y);
void wizard_projectile_draw(Projectile *projectile);
Rectangle wizard_projectile_hitbox(Projectile *projectile);

void enemy_wizard_initialize(Enemy *enemy) {
    enemy->texture = LoadTexture("assets/textures/enemies/wizard.png");
    enemy->music = LoadMusicStream("assets/music/boss/wizard.ogg");
    enemy->music.looping = true;

    enemy_wizard_projectile_texture = LoadTexture("assets/textures/enemies/projectile/wizard.png");
}

void enemy_wizard_unload(Enemy *enemy) {
    UnloadTexture(enemy->texture);
    UnloadMusicStream(enemy->music);

    UnloadTexture(enemy_wizard_projectile_texture);
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