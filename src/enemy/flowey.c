#include "enemy/type/flowey.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>
#include <stdlib.h>

#include "globals.h"
#include "items.h"
#include "enemy/enemy.h"

#include "player/player.h"
#include "asset_manager.h"
#include "enemy/projectile/projectile.h"

struct FloweyProjectileData {
    bool initialized;
    Vector2 velocity;
};

void spawn_flowey_projectile(struct Array *projectiles, int x, int y);
void flowey_projectile_draw(struct Projectile *projectile);
Rectangle flowey_projectile_hitbox(struct Projectile *projectile);

void enemy_flowey_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_flowey;

    if (enemy->music.capacity == 0) {
        array_allocate(&enemy->music, sizeof(Music *), 1);
        ((Music **) enemy->music.data)[0] = &assets.music_flowey;
        enemy->music.size = 1;
    }
}

void enemy_flowey_unload(Enemy *enemy) {

}

bool enemy_flowey_attack(struct Array *projectiles, int rand_type, float timer, int turn) {
    const int waves = (int) Clamp((float) turn + 2, 2, 5);


    // Spawn projectiles nicely in an arc
    if (timer < 0.05) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2) - 64;
        const int y = 48;

        while (projectiles->size < waves) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    } else if (timer < 0.1) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2) - 32;
        const int y = 32;

        while (projectiles->size < waves * 2) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    } else if (timer < 0.15) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2);
        const int y = 16;

        while (projectiles->size < waves * 3) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    } else if (timer < 0.2) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2) + 32;
        const int y = 32;

        while (projectiles->size < waves * 4) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    } else if (timer < 0.25) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2) + 64;
        const int y = 48;

        while (projectiles->size < waves * 5) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    }


    const int size = projectiles->size;
    struct Projectile *projectiles_data = (struct Projectile*)projectiles->data;
    for (int i = 0; i < size; i++) {
        struct Projectile *projectile = &projectiles_data[i];

        const float delay = floorf(i % waves) * 0.2f + 0.85f;

        if (timer > delay) {
            struct FloweyProjectileData *data = projectile->data;

            if (!data->initialized) {
                data->initialized = true;

                Vector2 vec = { player.position.x - (projectile->position.x + 8), player.position.y - (projectile->position.y + 8) };
                vec = Vector2Normalize(vec);

                data->velocity.x = vec.x;
                data->velocity.y = vec.y;
            }


            const float speed = Clamp((timer - delay) * 256.0f, 0, 512.0f) * GetFrameTime();

            projectile->position.x += data->velocity.x * speed;
            projectile->position.y += data->velocity.y * speed;
        }

    }

    return timer > 0.25f && size == 0;
}

void enemy_flowey_pre_defeat() {
    player.flags.boss_flowey = true;

    player_add_item(&player, (struct Item) {
        .type = COIN,
        .texture = &assets.texture_item_coin,
        .value = 1
    });
}

void enemy_flowey_post_defeat(struct Hud *hud) {
    hud_set_dialogue(hud, "Farmer Johan: As promised, I'll hand over the golden coin.");
}

void spawn_flowey_projectile(struct Array *projectiles, int x, int y) {
    const int size = projectiles->size;
    array_allocate(projectiles, sizeof(struct Projectile), size + 1);


    struct Projectile projectile;

    projectile.lifespan = 0;
    projectile.texture = &assets.texture_projectile_flowey;
    projectile.position = (Vector2){(float) x, (float) y};
    projectile.damage = 2;
    projectile.true_damage = false;
    projectile.data = calloc(sizeof(struct FloweyProjectileData), 1);

    projectile.hitbox = flowey_projectile_hitbox;
    projectile.draw = flowey_projectile_draw;
    projectile.on_hit = projectile_damage_player;

    ((struct Projectile*)projectiles->data)[size] = projectile;

    projectiles->size = size + 1;
}

void flowey_projectile_draw(struct Projectile *projectile) {
    int sprite = (int)floorf(projectile->lifespan * 20) % 2;
    int width = projectile->texture->width / 2;

    Rectangle rect = {(float) sprite * (float) width, 0, (float) width, (float) projectile->texture->height};

    DrawTextureRec(*projectile->texture, rect, (Vector2){projectile->position.x, projectile->position.y}, WHITE);
}

Rectangle flowey_projectile_hitbox(struct Projectile *projectile) {
    return (Rectangle){projectile->position.x, projectile->position.y, (float) projectile->texture->width / 2, (float) projectile->texture->height};
}