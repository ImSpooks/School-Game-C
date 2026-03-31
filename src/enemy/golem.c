#include "enemy/type/golem.h"

#include <config.h>
#include <math.h>
#include <stdio.h>
#include <raymath.h>
#include <stdlib.h>

#include "globals.h"
#include "items.h"
#include "enemy/enemy.h"

#include "player/player.h"
#include "scene/scene.h"
#include "asset_manager.h"
#include "enemy/projectile/projectile.h"
#include "scene/scene_battle.h"
#include "util/util.h"

struct Projectile* spawn_golem_projectile(struct Array *projectiles, int x, int y);
void golem_projectile_draw(struct Projectile *projectile);
Rectangle golem_projectile_hitbox(struct Projectile *projectile);

// Data needed for some attacks
float golem_attack_interval = 0;
int golem_attack_index = 0;

struct GolemProjectileData {
    Vector2 velocity;
    float rotation;
    int sprite;
};

bool golem_attack_1(struct Array *projectiles, float timer);
bool golem_attack_2(struct Array *projectiles, float timer);
bool golem_attack_3(struct Array *projectiles, float timer);

void enemy_golem_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_golem;

    if (enemy->music.capacity == 0) {
        array_allocate(&enemy->music, sizeof(Music*), 2);
        ((Music**) enemy->music.data)[0] = &assets.music_golem_intro;
        ((Music**) enemy->music.data)[1] = &assets.music_golem_loop;

        assets.music_golem_intro.looping = false;
        enemy->music.size = 2;
    }
}

void enemy_golem_unload(Enemy *enemy) {

}

bool enemy_golem_attack(struct Array *projectiles, int rand_type, float timer, int turn) {
    bool should_end = true;

    if (rand_type == 0) {
        should_end = golem_attack_1(projectiles, timer);
    } else if (rand_type == 1) {
        should_end = golem_attack_2(projectiles, timer);
    } else if (rand_type == 2) {
        should_end = golem_attack_3(projectiles, timer);
    }

    if (should_end) {
        golem_attack_interval = 0;
        golem_attack_index = 0;
    }

    return should_end;
}

void enemy_golem_pre_defeat() {
    player.flags.boss_golem = true;
}

void enemy_golem_post_defeat(struct Hud *hud) {

}

struct Projectile* spawn_golem_projectile(struct Array *projectiles, int x, int y) {
    Texture2D *texture = &assets.texture_projectile_golem;

    const int size = projectiles->size;
    array_allocate(projectiles, sizeof(struct Projectile), size + 1);

    struct Projectile projectile;

    projectile.lifespan = 0;
    projectile.texture = texture;
    projectile.position = (Vector2){(float) x, (float) y};
    projectile.damage = 10;
    projectile.true_damage = false;
    projectile.data = calloc(sizeof(struct GolemProjectileData), 1);

    projectile.draw = golem_projectile_draw;
    projectile.hitbox = golem_projectile_hitbox;
    projectile.on_hit = projectile_damage_player;

    struct GolemProjectileData *data = projectile.data;
    data->rotation = rand() % 360;
    data->sprite = rand() % 4;

    ((struct Projectile*)projectiles->data)[size] = projectile;

    projectiles->size = size + 1;

    return &((struct Projectile*)projectiles->data)[size];
}

void golem_projectile_draw(struct Projectile *projectile) {
    struct GolemProjectileData *data = projectile->data;

    const int sprite = data->sprite;
    const int width = projectile->texture->width / 4;

    DrawTexturePro(
        *projectile->texture,
        (Rectangle){sprite * width, 0, (float) width, (float) projectile->texture->height},
        (Rectangle){projectile->position.x, projectile->position.y, (float) width, (float) projectile->texture->height},

        (Vector2){(float) width / 2.0f, (float) projectile->texture->height / 2.0f},
        data->rotation,
        WHITE
    );

    // Rectangle rectangle = golem_projectile_hitbox(projectile);
    // DrawRectangleRec(rectangle, Fade(RED, 0.5f));
}

Rectangle golem_projectile_hitbox(struct Projectile *projectile) {
    struct GolemProjectileData *data = projectile->data;

    const int sprite = data->sprite;
    const int width = projectile->texture->width / 4;

    return (Rectangle){
        projectile->position.x - width / 2,
        projectile->position.y - projectile->texture->height / 2,
        width,
        projectile->texture->height
    };
}

bool golem_attack_1(struct Array *projectiles, float timer) {
    if (golem_attack_interval == 0) {
        bool right_side = rand() & 1;

        const float sprite_width = (float) assets.texture_projectile_golem.width / 4.0f / 2.0f;
        const float x = BATTLE_BOUNDS.x + (right_side ? BATTLE_BOUNDS.width - sprite_width : sprite_width);
        const float y = BATTLE_BOUNDS.y + BATTLE_BOUNDS.height - (BATTLE_BOUNDS.height / 5.0f * (float) ((double) rand() / (double) RAND_MAX));

        struct Projectile *projectile = spawn_golem_projectile(projectiles, (int) x, (int) y);

        struct GolemProjectileData *data = projectile->data;

        const float velocity_x = 2.5f + (float) ((double) rand() / (double) RAND_MAX * 2.5f) ;
        data->velocity = (Vector2){
            (right_side ? -velocity_x : velocity_x) ,
            7.0f + (float) ((double) rand() / (double) RAND_MAX) * 5.0f
        };
    }

    const float frame_time = GetFrameTime();

    golem_attack_interval += frame_time;
    // x projectiles per second
    if (golem_attack_interval > 1.0f / 2.0f) {
        golem_attack_interval = 0;
     }


    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct GolemProjectileData *data = projectile->data;

        // Old game was 0.995 per 20ms (50 ticks/s), 0.995^50 = 0.7783 units/s
        data->velocity.x = data->velocity.x * powf(0.7783f, frame_time);
        // Old game was y - 0.3 per 20ms (50 ticks/s), 0.3 * 50 = 15 units/s
        data->velocity.y -= 15.0f * frame_time;

        projectile->position.x += data->velocity.x * frame_time * 30.0f;
        projectile->position.y -= data->velocity.y * frame_time * 30.0f;

        if (data->velocity.x > 0) {
            // going to the right
            if (projectile->position.x > BATTLE_BOUNDS.x + BATTLE_BOUNDS.width - ((float) projectile->texture->width / 4.0f / 2.0f)) {
                projectile_despawn(projectile);
            }
        } else {
            // going to the left
            if (projectile->position.x < BATTLE_BOUNDS.x + ((float) projectile->texture->width / 4.0f / 2.0f)) {
                projectile_despawn(projectile);
            }
        }

        if (projectile->position.y < BATTLE_BOUNDS.y + ((float) projectile->texture->height / 2.0f) && data->velocity.y > 0) {
            data->velocity.y *= -1;
        } else if (projectile->position.y > BATTLE_BOUNDS.y + BATTLE_BOUNDS.height - ((float) projectile->texture->height / 2.0f) && data->velocity.y < 0) {
            // make it bounce based in current velocity
            data->velocity.y *= -1 * (0.8f + ((float) ((double) rand() / (double) RAND_MAX * 2) - 1.0f) * 0.05f);
        }
    }

    return timer >= 7.0f;
}

bool golem_attack_2(struct Array *projectiles, float timer) {
    const float frame_time = GetFrameTime();

    golem_attack_interval += frame_time;
    // x projectiles per second
    if (golem_attack_interval > 1.0f / 30.0f && timer < 3.6f) {
        golem_attack_interval -= 1.0f / 30.0f;
        golem_attack_index++;

        const float x = BATTLE_BOUNDS.x + (BATTLE_BOUNDS.width / 2.0f);
        const float y = 16 + 4 + (float) assets.texture_enemy_golem.height / 2.0f;

        struct Projectile *projectile = spawn_golem_projectile(projectiles, (int) x, (int) y);

        const float rotation = fmodf(
            180.0f + ((float) golem_attack_index * 20.0f) + (float)((double) rand() / (double) RAND_MAX * 40.0),
            360.0f
        );

        const float deg2rad_rotation = DEG2RAD * rotation;
        const float speed = 0.5f + timer / 2.0f;

        struct GolemProjectileData *data = projectile->data;

        data->velocity = (Vector2){cosf(deg2rad_rotation) * speed, sinf(deg2rad_rotation) * speed};
    }


    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct GolemProjectileData *data = projectile->data;

        projectile->position.x += data->velocity.x * frame_time * 100.0f;
        projectile->position.y -= data->velocity.y * frame_time * 100.0f;
    }

    return timer > 5.1f;
}

int golem_attack_3_index = 0;

bool golem_attack_3(struct Array *projectiles, float timer) {
    const float frame_time = GetFrameTime();

    golem_attack_interval += frame_time;

    if (golem_attack_3_index < 6 && golem_attack_interval > 1.0f / 14.0f) {
        golem_attack_interval -= 1.0f / 14.0f;

        int angles_length;
        float *angles;
        if (golem_attack_index % 2 == 0) {
            angles = (float[]){-40.0f, -20.0f, 0.0f, 20.0f, 40.0f};
            angles_length = 5;
        } else {
            angles = (float[]){-50.0f, -30.0f, -10.0f, 10.0f, 30.0f, 50.0f};
            angles_length = 6;
        }

        const float x = BATTLE_BOUNDS.x + (BATTLE_BOUNDS.width / 2.0f);
        const float y = 16 + 4 + (float) assets.texture_enemy_golem.height / 2.0f;

        const float angle = atan2f(x - player.position.x, y - player.position.y);

        for (int i = 0; i < angles_length; i++) {
            struct Projectile *projectile = spawn_golem_projectile(projectiles, (int) x, (int) y);

            const float deg2rad_rotation = angle + (angles[i] + 90) * DEG2RAD;

            struct GolemProjectileData *data = projectile->data;
            data->velocity = (Vector2) {cosf(deg2rad_rotation), sinf(deg2rad_rotation)};
        }

        golem_attack_3_index++;
    }

    if (golem_attack_interval > 0.4f && ++golem_attack_index < 6) {
        golem_attack_interval -= 0.4f;
        golem_attack_3_index = 0;
    }

    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct GolemProjectileData *data = projectile->data;

        projectile->position.x += data->velocity.x * frame_time * 256.0f;
        projectile->position.y -= data->velocity.y * frame_time * 256.0f;
    }

    if (timer > (0.75f) * 6.0f + 1.5f) {
        golem_attack_3_index = 0;
        return true;
    }
    return false;
}