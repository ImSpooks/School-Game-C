#include "enemy/type/renoir.h"

#include "asset_manager.h"
#include "player/player.h"

struct Projectile* spawn_renoir_projectile(struct Array *projectiles, int x, int y);
void renoir_projectile_draw(struct Projectile *projectile);
Rectangle renoir_projectile_hitbox(struct Projectile *projectile);
void renoir_projectile_explode(struct Projectile *projectile);

float renoir_attack_interval = 0;
int renoir_attack_index = 0;

struct RenoirProjectileData {
    Vector2 velocity;
    float rotation;
    
};

bool renoir_attack_1(struct Array *projectiles, float timer, bool first_tick);
bool renoir_attack_2(struct Array *projectiles, float timer, bool first_tick);
bool renoir_attack_3(struct Array *projectiles, float timer, bool first_tick);


void enemy_renoir_initialize(Enemy* enemy) {
    enemy->texture = &assets.texture_enemy_renoir;

    if (enemy->music.capacity == 0) {
        array_allocate(&enemy->music, sizeof(Music*), 2);
        ((Music**) enemy->music.data)[0] = &assets.music_renoir_intro;
        ((Music**) enemy->music.data)[1] = &assets.music_renoir_loop;

        enemy->music.size = 2;
    }
}

void enemy_renoir_unload(Enemy* enemy) {

}


bool enemy_renoir_attack(struct Array *projectiles, int rand_type, float timer, int turn, bool first_tick) {
    bool should_end = true;

    if (rand_type == 0) {
        should_end = renoir_attack_1(projectiles, timer, first_tick);
    } else if (rand_type == 1) {
        should_end = renoir_attack_2(projectiles, timer, first_tick);
    } else if (rand_type == 2) {
        should_end = renoir_attack_3(projectiles, timer, first_tick);
    }

    if (should_end) {
        renoir_attack_interval = 0;
        renoir_attack_index = 0;
        return true;
    }

    return false;
}

void enemy_renoir_pre_defeat() {
    player.flags.boss_renoir = true;
}

void enemy_renoir_post_defeat(struct Hud *hud) {

}


bool renoir_attack_1(struct Array *projectiles, float timer, bool first_tick) {


    return true;
}

bool renoir_attack_2(struct Array *projectiles, float timer, bool first_tick) {
    return true;
}

bool renoir_attack_3(struct Array *projectiles, float timer, bool first_tick) {
    return true;
}