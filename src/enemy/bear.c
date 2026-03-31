#include "enemy/type/bear.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>
#include <stdlib.h>

#include "globals.h"
#include "items.h"
#include "enemy/enemy.h"
#include "scene/scene_battle.h"

#include "player/player.h"
#include "asset_manager.h"
#include "enemy/projectile/projectile.h"

struct Projectile* spawn_bear_projectile(struct Array *projectiles, int x, int y);
void bear_projectile_draw(struct Projectile *projectile);
Rectangle bear_projectile_hitbox(struct Projectile *projectile);


// Data needed for some attacks
float bear_attack_interval = 0;
int bear_attack_index = 0;

struct BearProjectileData {
    Vector2 velocity;
    float rotation;
};

bool bear_attack_1(struct Array *projectiles, float timer);
bool bear_attack_2(struct Array *projectiles, float timer);
bool bear_attack_3(struct Array *projectiles, float timer);

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
    bool should_end = true;

    if (rand_type == 0) {
        should_end = bear_attack_1(projectiles, timer);
    } else if (rand_type == 1) {
        should_end = bear_attack_2(projectiles, timer);
    } else if (rand_type == 2) {
        should_end = bear_attack_3(projectiles, timer);
    }

    if (should_end) {
        bear_attack_interval = 0;
        bear_attack_index = 0;
    }

    return should_end;
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

void bear_projectile_draw(struct Projectile *projectile) {
    struct BearProjectileData *data = projectile->data;

    DrawTexturePro(
        *projectile->texture,
        (Rectangle){0, 0, (float) projectile->texture->width, (float) projectile->texture->height},
        (Rectangle){projectile->position.x, projectile->position.y, (float) projectile->texture->width, (float) projectile->texture->height},

        (Vector2){(float) projectile->texture->width / 2.0f, (float) projectile->texture->height / 2.0f},
        data->rotation,
        WHITE
    );

    // Rectangle rectangle = bear_projectile_hitbox(projectile);
    // DrawRectangleRec(rectangle, Fade(RED, 0.5f));
}

Rectangle bear_projectile_hitbox(struct Projectile *projectile) {
    return (Rectangle){
        projectile->position.x - projectile->texture->width / 2,
        projectile->position.y - projectile->texture->height / 2,
        projectile->texture->width,
        projectile->texture->height
    };
}

struct Projectile* spawn_bear_projectile(struct Array *projectiles, int x, int y) {
    Texture2D *texture = &assets.texture_projectile_bear;

    const int size = projectiles->size;
    array_allocate(projectiles, sizeof(struct Projectile), size + 1);

    struct Projectile projectile;

    projectile.lifespan = 0;
    projectile.texture = texture;
    projectile.position = (Vector2){(float) x, (float) y};
    projectile.damage = 2;
    projectile.true_damage = false;
    projectile.data = calloc(sizeof(struct BearProjectileData), 1);

    projectile.draw = bear_projectile_draw;
    projectile.hitbox = bear_projectile_hitbox;
    projectile.on_hit = projectile_damage_player;

    ((struct Projectile*)projectiles->data)[size] = projectile;

    projectiles->size = size + 1;

    return &((struct Projectile*)projectiles->data)[size];
}

bool bear_attack_1(struct Array *projectiles, float timer) {
    // x projectiles per second
    if (bear_attack_interval >= 1.0f / 8.0f) {
        bear_attack_interval -= 1.0f / 8.0f;

        bool upside_down = rand() & 1;

        Texture2D *texture = &assets.texture_projectile_bear;

        const float min_x = BATTLE_BOUNDS.x - (float) texture->width;
        const float x = min_x + (BATTLE_BOUNDS.width * (float) ((double) rand() / (double) RAND_MAX)) + (float) texture->width;
        const float y = upside_down ? BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture->height : BATTLE_BOUNDS.y - texture->height;

        struct Projectile *projectile = spawn_bear_projectile(projectiles, (int) x, (int) y);

        struct BearProjectileData *data = projectile->data;
        data->velocity = (Vector2){0, upside_down ? -1.0f : 1.0f};
        data->rotation = upside_down ? 180 : 0;
    }

    bear_attack_interval += GetFrameTime();

    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct BearProjectileData *data = projectile->data;
        const float speed = Clamp(projectile->lifespan * 222.0f, 0, 280.0f) * GetFrameTime();

        projectile->position.y += data->velocity.y * speed;

        bool upside_down = data->rotation > 0;
        if (upside_down && projectile->position.y < BATTLE_BOUNDS.y - projectile->texture->height) {
            projectile_despawn(projectile);
        } else if (!upside_down && projectile->position.y > BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + projectile->texture->height) {
            projectile_despawn(projectile);
        }
    }

    return timer >= 6.0f;
}


bool bear_attack_2(struct Array *projectiles, float timer) {
    Vector2 locations[] = {
        {225, 70},
        {225, -20},
        {225, -110},

        {0, -130},

        {-225, -110},
        {-225, -20},
        {-225, 70},
    };

    const int length = sizeof(locations) / sizeof(locations[0]);

    const int directions = 12;

    // x times per second
    if (bear_attack_interval >= 1.0f / 3.0f) {
        bear_attack_interval = 0;

        Vector2 vector = locations[bear_attack_index++ % length];

        const float added_angle = 360.0f / (float) directions * (float) ((double) rand() / (double) RAND_MAX);

        for (int i = 0; i < directions; i++) {
            const float x = BATTLE_BOUNDS_MIDDLE.x + vector.x;
            const float y = BATTLE_BOUNDS_MIDDLE.y + vector.y;

            struct Projectile *projectile = spawn_bear_projectile(projectiles, (int) x, (int) y);

            float rotation = ((float) i / (float) directions) * 360.0f + added_angle;
            if (rotation > 360)
                rotation -= 360; // just to be sure

            const float deg2rad_rotation = DEG2RAD * rotation;

            struct BearProjectileData *data = projectile->data;
            data->velocity = (Vector2){cosf(deg2rad_rotation), sinf(deg2rad_rotation)};
            data->rotation = rotation + 90;
        }
    }

    bear_attack_interval += GetFrameTime();

    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct BearProjectileData *data = projectile->data;
        const float speed = Clamp(projectile->lifespan * 90.0f, 0, 160.0f) * GetFrameTime();

        projectile->position.x += data->velocity.x * speed;
        projectile->position.y += data->velocity.y * speed;
    }

    return timer >= 8.0f;
}

bool bear_attack_3(struct Array *projectiles, float timer) {
    const Vector2 locations[] = {
        {225, 70},
        {225, -110},

        {0, -140},

        {-225, -110},
        {-225, 70},
    };

    const int length = sizeof(locations) / sizeof(locations[0]);

    const int directions = 8;

    // x times per second
    if (bear_attack_interval == 0) {
        for (int i = 0; i < length; i++) {
            Vector2 vector = locations[i];

            const float added_angle = 360.0f / (float) directions * (float) ((double) rand() / (double) RAND_MAX);

            for (int ii = 0; ii < directions; ii++) {
                const float x = BATTLE_BOUNDS_MIDDLE.x + vector.x;
                const float y = BATTLE_BOUNDS_MIDDLE.y + vector.y;

                struct Projectile *projectile = spawn_bear_projectile(projectiles, (int) x, (int) y);

                float rotation = ((float) ii / (float) directions) * 360.0f + added_angle;
                if (rotation > 360)
                    rotation -= 360; // just to be sure

                struct BearProjectileData *data = projectile->data;
                data->velocity = (Vector2){cosf(DEG2RAD * rotation), sinf(DEG2RAD * rotation)};
                data->rotation = rotation + 90;
            }
        }
    }
    bear_attack_interval += GetFrameTime();

    if (bear_attack_interval > 1.35f) {
        bear_attack_interval = 0;
    }

    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct BearProjectileData *data = projectile->data;
        const float speed = Clamp(projectile->lifespan * 80.0f, 0, 160.0f) * GetFrameTime();

        projectile->position.x += data->velocity.x * speed;
        projectile->position.y += data->velocity.y * speed;
    }

    return timer >= 8.0f;
}