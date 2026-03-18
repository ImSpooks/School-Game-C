#include "flowey.h"

#include "enemy.h"
#include "../../player/player.h"
#include "../../screen/screen.h"

Texture2D enemy_flowey_projectile_texture;

void enemy_flowey_initialize(Enemy* enemy) {
    enemy->texture = LoadTexture("assets/textures/enemies/flowey.png");
    enemy->music = LoadMusicStream("assets/music/boss/flowey.ogg");
    enemy->music.looping = true;

    enemy_flowey_projectile_texture = LoadTexture("assets/textures/enemies/projectile/flowey.png");
}

void enemy_flowey_unload(Enemy* enemy) {
    UnloadTexture(enemy->texture);
    UnloadMusicStream(enemy->music);

    UnloadTexture(enemy_flowey_projectile_texture);
}

bool enemy_flowey_attack(Projectile* vector, int type, float timer, int turn) {
    return timer > 1.0f;
}

void enemy_flowey_defeat() {
    player.flags.boss_flowey = true;
    setDialogue("Farmer Johan: As promised, I'll hand over the 50 gold.");
}