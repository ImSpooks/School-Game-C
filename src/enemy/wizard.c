#include "enemy/type/wizard.h"

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
#include "scene/scene_battle.h"

enum WizardProjectileType {
    ORB,
    HEAL_ORB,
    TORNADO,
};

struct Projectile* spawn_wizard_projectile(struct Array *projectiles, int x, int y, enum WizardProjectileType type);
void wizard_projectile_draw(struct Projectile *projectile);
Rectangle wizard_projectile_hitbox(struct Projectile *projectile);
void projectile_push_player(struct Projectile *projectile);

float wizard_attack_interval = 0;
int wizard_attack_index = 0;


struct WizardProjectileData {
    Vector2 velocity;

    //attack 1
    bool is_right_spawn;
    
    // attack 2
    bool initialized;

    // attack 3
    enum WizardProjectileType type;
    int tornado_sprite;
    float tornado_sprite_timer;
};

bool wizard_attack_1(struct Array *projectiles, float timer);
bool wizard_attack_2(struct Array *projectiles, float timer);
bool wizard_attack_3(struct Array *projectiles, float timer);
bool wizard_attack_4(struct Array *projectiles, float timer);

void enemy_wizard_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_wizard;

    if (enemy->music.capacity == 0) {
        array_allocate(&enemy->music, sizeof(Music*), 1);
        ((Music**) enemy->music.data)[0] = &assets.music_wizard;
        enemy->music.size = 1;
    }
}

void enemy_wizard_unload(Enemy *enemy) {

}

bool enemy_wizard_attack(struct Array *projectiles, int rand_type, float timer, int turn) {
    bool should_end = true;

    rand_type = 2;

    if (rand_type == 0) {
        should_end = wizard_attack_1(projectiles, timer);
    } else if (rand_type == 1) {
        should_end = wizard_attack_2(projectiles, timer);
    } else if (rand_type == 2) {
        should_end = wizard_attack_3(projectiles, timer);
    } else if (rand_type == 3) {
        should_end = wizard_attack_4(projectiles, timer);
    }


    if (should_end) {
        wizard_attack_interval = 0;
        wizard_attack_index = 0;
    }

    return should_end;
}

void enemy_wizard_pre_defeat() {
    player.flags.boss_wizard = true;
}

void enemy_wizard_post_defeat(struct Hud *hud) {

}

struct Projectile* spawn_wizard_projectile(struct Array *projectiles, int x, int y, enum WizardProjectileType type) {

    const int size = projectiles->size;
    array_allocate(projectiles, sizeof(struct Projectile), size + 1);

    struct Projectile projectile;

    projectile.lifespan = 0;
    projectile.position = (Vector2){(float) x, (float) y};
    projectile.true_damage = false;
    projectile.data = calloc(sizeof(struct WizardProjectileData), 1);

    projectile.draw = wizard_projectile_draw;
    projectile.hitbox = wizard_projectile_hitbox;

    struct WizardProjectileData *data = projectile.data;

    data->type = type;
    switch (type) {
        case ORB:
            projectile.damage = 16;
            projectile.texture = &assets.texture_projectile_wizard_orb;
            projectile.on_hit = projectile_damage_player;
            break;

        case HEAL_ORB:
            projectile.damage = 4;
            projectile.texture = &assets.texture_projectile_wizard_heal_orb;
            projectile.on_hit = projectile_heal_player;
            break;

        case TORNADO:
            projectile.damage = 0;
            projectile.texture = &assets.texture_projectile_wizard_tornado;
            projectile.on_hit = projectile_damage_player;
            break;
    }

    // ReSharper disable once CppSomeObjectMembersMightNotBeInitialized
    ((struct Projectile*)projectiles->data)[size] = projectile;

    projectiles->size = size + 1;

    return &((struct Projectile*)projectiles->data)[size];

}

void wizard_projectile_draw(struct Projectile *projectile) {
    struct WizardProjectileData *data = projectile->data;

    switch (data->type) {
        case ORB:
        case HEAL_ORB:

            DrawTexturePro(
                *projectile->texture,
                (Rectangle){0, 0, (float) projectile->texture->width, (float) projectile->texture->height},
                (Rectangle){projectile->position.x, projectile->position.y, (float) projectile->texture->width, (float) projectile->texture->height},

                (Vector2){(float) projectile->texture->width / 2.0f, (float) projectile->texture->height / 2.0f},
                0,
                WHITE
            );
            break;

        case TORNADO: {
            const int sprite = data->tornado_sprite;
            const int width = projectile->texture->width / 4;

            DrawTexturePro(
                *projectile->texture,
                (Rectangle){(float) sprite * (float) width, 0, (float) width, (float) projectile->texture->height},
                (Rectangle){projectile->position.x, projectile->position.y, (float) width, (float) projectile->texture->height},

                (Vector2){(float) width / 2.0f, (float) projectile->texture->height / 2.0f},
                0,
                WHITE
            );
            break;
        }
    }

    // Rectangle rectangle = wizard_projectile_hitbox(projectile);
    // DrawRectangleRec(rectangle, Fade(RED, 0.5f));
}

Rectangle wizard_projectile_hitbox(struct Projectile *projectile) {
    struct WizardProjectileData *data = projectile->data;

    switch (data->type) {
        case ORB:
        case HEAL_ORB:

            return (Rectangle){
                projectile->position.x - projectile->texture->width / 2 + 1,
                projectile->position.y - projectile->texture->height / 2 + 1,
                projectile->texture->width - 2,
                projectile->texture->height - 2
            };

        case TORNADO:
            const int width = projectile->texture->width / 4;

            return (Rectangle){
                projectile->position.x - width / 2,
                projectile->position.y - projectile->texture->height / 2,
                width,
                projectile->texture->height
            };
    }
}

bool wizard_attack_1(struct Array *projectiles, float timer) {
    // x projectiles per second
    if (wizard_attack_interval >= 1.0f / 12.0f) {
        wizard_attack_interval -= 1.0f / 12.0f;

        bool is_right_spawn = rand() & 1;

        Texture2D *texture = &assets.texture_projectile_wizard_orb;

        const float x = is_right_spawn ? BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture->width : BATTLE_BOUNDS.x - texture->width;

        const float min_y = BATTLE_BOUNDS.y - (float) texture->height;
        const float y = min_y + (BATTLE_BOUNDS.height * (float) ((double) rand() / (double) RAND_MAX)) + (float) texture->height;

        enum WizardProjectileType type = rand() < RAND_MAX / 10 ? HEAL_ORB : ORB;

        struct Projectile *projectile = spawn_wizard_projectile(projectiles, (int) x, (int) y, type);

        struct WizardProjectileData *data = projectile->data;
        data->velocity = (Vector2){is_right_spawn ? -1.0f : 1.0f, 0};
        data->is_right_spawn = is_right_spawn;
    }

    wizard_attack_interval += GetFrameTime();

    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct WizardProjectileData *data = projectile->data;
        const float speed = Clamp(projectile->lifespan * 222.0f, 0, 280.0f) * GetFrameTime();

        projectile->position.x += data->velocity.x * speed;

        if (data->is_right_spawn && projectile->position.x < BATTLE_BOUNDS.x - (float) projectile->texture->width) {
            projectile_despawn(projectile);
        } else if (!data->is_right_spawn && projectile->position.x > BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + (float) projectile->texture->width) {
            projectile_despawn(projectile);
        }
    }

    return timer >= 6.0f;
}

int wizard_attack_2_index = 0;
int wizard_attack_2_type = 0;
bool wizard_attack_2(struct Array *projectiles, float timer) {

    float texture_width = (float) assets.texture_projectile_wizard_orb.width;
    float texture_height = (float) assets.texture_projectile_wizard_orb.height;

    const Vector2 line_1[6] = {
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 0), BATTLE_BOUNDS.y - texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1.0f / 5.0f), BATTLE_BOUNDS.y - texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1.0f / 5.0f * 2), BATTLE_BOUNDS.y - texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1.0f / 5.0f * 3), BATTLE_BOUNDS.y - texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1.0f / 5.0f * 4), BATTLE_BOUNDS.y - texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1), BATTLE_BOUNDS.y - texture_height}
    };

    const Vector2 line_2[6] = {
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 0), BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1.0f / 5.0f), BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1.0f / 5.0f * 2), BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1.0f / 5.0f * 3), BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1.0f / 5.0f * 4), BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_height},
        {Lerp(BATTLE_BOUNDS.x - texture_width, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, 1), BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_height}
    };

    const Vector2 line_3[6] = {
        {BATTLE_BOUNDS.x - texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 0)},
        {BATTLE_BOUNDS.x - texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f / 5.0f)},
        {BATTLE_BOUNDS.x - texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f / 5.0f * 2)},
        {BATTLE_BOUNDS.x - texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f / 5.0f * 3)},
        {BATTLE_BOUNDS.x - texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f / 5.0f * 4)},
        {BATTLE_BOUNDS.x - texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f)},
    };

    const Vector2 line_4[6] = {
        {BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 0)},
        {BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f / 5.0f)},
        {BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f / 5.0f * 2)},
        {BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f / 5.0f * 3)},
        {BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f / 5.0f * 4)},
        {BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture_width, Lerp(BATTLE_BOUNDS.y - texture_width, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + texture_width, 1.0f)},
    };
    
    const float frame_time = GetFrameTime();
    wizard_attack_interval += frame_time;
    
    if (wizard_attack_2_index < 6 && wizard_attack_interval > 1.0f / 12.0f) {
        wizard_attack_interval -= 1.0f / 12.0f;

        const Vector2 *line;
        if (wizard_attack_2_type % 4 == 0) {
            line = line_1;
        } else if (wizard_attack_2_type % 4 == 1) {
            line = line_2;
        } else if (wizard_attack_2_type % 4 == 2) {
            line = line_3;
        } else {
            line = line_4;
        }

        bool left_ro_right = wizard_attack_2_type < 4;

        Vector2 location = line[left_ro_right ? wizard_attack_2_index : 5 - wizard_attack_2_index];


        spawn_wizard_projectile(projectiles, (int) location.x, (int) location.y, ORB);

        wizard_attack_2_index++;
    }
    
    if (wizard_attack_interval > 0.8f - 1.0f / 12.0f * 6 && ++wizard_attack_index < 6) {
        wizard_attack_interval -= 0.4f;
        wizard_attack_2_index = 0;
        wizard_attack_2_type = (int) floor((double) rand() / (double) RAND_MAX * 8);
    }
    
    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        if (projectile->lifespan > 0.1f) {
            struct WizardProjectileData *data = projectile->data;

            if (!data->initialized) {
                data->initialized = true;

                Vector2 vec = { player.position.x - (projectile->position.x + 8), player.position.y - (projectile->position.y + 8) };
                vec = Vector2Normalize(vec);

                data->velocity.x = vec.x;
                data->velocity.y = vec.y;
            }

            const float speed = Clamp((projectile->lifespan - 0.1f) * 64.0f, 0, 256.0f) * GetFrameTime();

            projectile->position.x += data->velocity.x * speed;
            projectile->position.y += data->velocity.y * speed;
        }
    }

    if (timer > (0.8f) * 6.0f + 1.5f) {
        wizard_attack_2_index = 0;
        wizard_attack_2_type = (int) floor((double) rand() / (double) RAND_MAX * 8);
        return true;
    }
    return false;
}

float wizard_attack_3_interval = 0;

bool wizard_attack_3(struct Array *projectiles, float timer) {
    const float frame_time = GetFrameTime();

    wizard_attack_interval += frame_time;
    wizard_attack_3_interval += frame_time;

    // x projectiles per second
    if (wizard_attack_interval >= 1.0f / 20.0f) {
        wizard_attack_interval -= 1.0f / 20.0f;

        bool is_right_spawn = rand() & 1;

        Texture2D *texture = &assets.texture_projectile_wizard_orb;

        const float x = is_right_spawn ? BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + texture->width : BATTLE_BOUNDS.x - texture->width;

        const float min_y = BATTLE_BOUNDS.y - (float) texture->height;
        const float y = min_y + (BATTLE_BOUNDS.height / 4.0f * (float) ((double) rand() / (double) RAND_MAX)) + (float) texture->height;

        struct Projectile *projectile = spawn_wizard_projectile(projectiles, (int) x, (int) y, ORB);

        struct WizardProjectileData *data = projectile->data;
        data->velocity = (Vector2){is_right_spawn ? -1.0f : 1.0f, 0};
        data->is_right_spawn = is_right_spawn;
    }

    if (wizard_attack_3_interval >= 1.0f / 6.0f) {
        wizard_attack_3_interval -= 1.0f / 6.0f;

        Texture2D *texture = &assets.texture_projectile_wizard_tornado;

        const float min_x = BATTLE_BOUNDS.x - (float) texture->width;
        const float x = min_x + (BATTLE_BOUNDS.width * (float) ((double) rand() / (double) RAND_MAX)) + (float) texture->width;
        const float y = BATTLE_BOUNDS.y + BATTLE_BOUNDS.height + (float) texture->height / 2.0f;

        struct Projectile *projectile = spawn_wizard_projectile(projectiles, (int) x, (int) y, TORNADO);
        projectile->on_hit = projectile_push_player;

        struct WizardProjectileData *data = projectile->data;
        data->velocity = (Vector2){0, 1.0f};
    }

    wizard_attack_interval += frame_time;

    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct WizardProjectileData *data = projectile->data;

        if (data->type == ORB) {
            const float speed = Clamp(projectile->lifespan * 222.0f, 0, 280.0f) * frame_time;

            projectile->position.x += data->velocity.x * speed;

            if (data->is_right_spawn && projectile->position.x < BATTLE_BOUNDS.x - (float) projectile->texture->width) {
                projectile_despawn(projectile);
            } else if (!data->is_right_spawn && projectile->position.x > BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + (float) projectile->texture->width) {
                projectile_despawn(projectile);
            }
        } else {
            // tornado
            const float speed = Clamp(projectile->lifespan * 96.0f, 0, 320.0f) * frame_time;

            projectile->position.y -= data->velocity.y * speed;

            if (projectile->position.y < BATTLE_BOUNDS.y - (float) projectile->texture->height) {
                projectile_despawn(projectile);
            }

            data->tornado_sprite_timer += frame_time;
            if (data->tornado_sprite_timer >= 0.1f) {
                data->tornado_sprite_timer -= 0.1f;

                data->tornado_sprite++;
                if (data->tornado_sprite > 3)
                    data->tornado_sprite = 0;
            }
        }
    }
    return timer >= 6.0f;
}

void projectile_push_player(struct Projectile *projectile) {
    const float frame_time = GetFrameTime();

    struct WizardProjectileData *data = projectile->data;
    const float speed = Clamp(projectile->lifespan * 96.0f, 0, 320.0f) * frame_time;

    player.position.y -= __max(PLAYER_SPEED * frame_time * 0.8, speed);
}

bool wizard_attack_4(struct Array *projectiles, float timer) {

}