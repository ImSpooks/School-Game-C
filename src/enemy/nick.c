#include "enemy/type/nick.h"

#include "asset_manager.h"
#include "player/player.h"

struct Projectile* spawn_nick_projectile(struct Array *projectiles, int x, int y);
void nick_projectile_draw(struct Projectile *projectile);
Rectangle nick_projectile_hitbox(struct Projectile *projectile);
void nick_projectile_explode(struct Projectile *projectile);

float nick_attack_interval = 0;
int nick_attack_index = 0;

struct NickProjectileData {
    Vector2 velocity;
    float rotation;
    
};

bool nick_attack_1(struct Array *projectiles, float timer, bool first_tick);
bool nick_attack_2(struct Array *projectiles, float timer, bool first_tick);
bool nick_attack_3(struct Array *projectiles, float timer, bool first_tick);

void enemy_nick_initialize(Enemy* enemy) {
    enemy->texture = &assets.texture_enemy_nick;

    if (enemy->music.capacity == 0) {
        array_allocate(&enemy->music, sizeof(Music *), 1);
        ((Music **) enemy->music.data)[0] = &assets.music_nick;
        enemy->music.size = 1;
    }
}

void enemy_nick_unload(Enemy* enemy) {

}

bool enemy_nick_attack(struct Array *projectiles, int rand_type, float timer, int turn, bool first_tick) {
    bool should_end = true;

    if (rand_type == 0) {
        should_end = nick_attack_1(projectiles, timer, first_tick);
    } else if (rand_type == 1) {
        should_end = nick_attack_2(projectiles, timer, first_tick);
    } else if (rand_type == 2) {
        should_end = nick_attack_3(projectiles, timer, first_tick);
    }

    if (should_end) {
        nick_attack_interval = 0;
        nick_attack_index = 0;
        return true;
    }

    return false;
}

void enemy_nick_pre_defeat() {
    player.flags.boss_nick = true;
}

void enemy_nick_post_defeat(struct Hud *hud) {

}

bool nick_attack_1(struct Array *projectiles, float timer, bool first_tick) {
    return true;
}

bool nick_attack_2(struct Array *projectiles, float timer, bool first_tick) {
    return true;
}

bool nick_attack_3(struct Array *projectiles, float timer, bool first_tick) {
    return true;
}