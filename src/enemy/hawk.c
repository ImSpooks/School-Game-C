#include "enemy/type/hawk.h"

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
#include "../../cmake-build-release/_deps/raylib-src/src/config.h"
#include "enemy/projectile/projectile.h"
#include "scene/scene_battle.h"

enum HawkProjectileType {
    BIRD,
    FEATHER,
    EGG,
};

struct Projectile* spawn_hawk_projectile(struct Array *projectiles, int x, int y, enum HawkProjectileType type);
void hawk_projectile_draw(struct Projectile *projectile);
Rectangle hawk_projectile_hitbox(struct Projectile *projectile);
void hawk_projectile_explode(struct Projectile *projectile);

float hawk_attack_interval = 0;
int hawk_attack_index = 0;

struct HawkProjectileData {
    Vector2 velocity;
    float rotation;
    enum HawkProjectileType type;

    //attack 1
    bool is_right_spawn;

    // attack 2
    float target_y;

    // attack 3
    Vector2 destination;
};

bool hawk_attack_1(struct Array *projectiles, float timer, bool first_tick);
bool hawk_attack_2(struct Array *projectiles, float timer, bool first_tick);
bool hawk_attack_3(struct Array *projectiles, float timer, bool first_tick);

void enemy_hawk_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_hawk;

    if (enemy->music.capacity == 0) {
        array_allocate(&enemy->music, sizeof(Music*), 2);
        ((Music**) enemy->music.data)[0] = &assets.music_hawk_intro;
        ((Music**) enemy->music.data)[1] = &assets.music_hawk_loop;

        assets.music_hawk_intro.looping = false;
        enemy->music.size = 2;
    }
}

void enemy_hawk_unload(Enemy *enemy) {

}

bool enemy_hawk_attack(struct Array *projectiles, int rand_type, float timer, int turn, bool first_tick) {
    bool should_end = true;
    
    rand_type = 2;

    if (rand_type == 0) {
        should_end = hawk_attack_1(projectiles, timer, first_tick);
    } else if (rand_type == 1) {
        should_end = hawk_attack_2(projectiles, timer, first_tick);
    } else if (rand_type == 2) {
        should_end = hawk_attack_3(projectiles, timer, first_tick);
    }

    if (should_end) {
        hawk_attack_interval = 0;
        hawk_attack_index = 0;
    }

    return should_end;
}

void enemy_hawk_pre_defeat() {
    player.flags.boss_hawk = true;
}

void enemy_hawk_post_defeat(struct Hud *hud) {

}

struct Projectile* spawn_hawk_projectile(struct Array *projectiles, int x, int y, enum HawkProjectileType type) {
    const int size = projectiles->size;
    array_allocate(projectiles, sizeof(struct Projectile), size + 1);

    struct Projectile projectile;

    projectile.lifespan = 0;
    projectile.position = (Vector2){(float) x, (float) y};
    projectile.true_damage = false;
    projectile.damage = 18;
    projectile.data = calloc(sizeof(struct HawkProjectileData), 1);

    projectile.draw = hawk_projectile_draw;
    projectile.hitbox = hawk_projectile_hitbox;
    projectile.on_hit = projectile_damage_player;

    struct HawkProjectileData *data = projectile.data;

    data->type = type;
    switch (data->type) {
        case BIRD:
            projectile.texture = &assets.texture_projectile_hawk_bird;
            break;
        case FEATHER:
            projectile.texture = &assets.texture_projectile_hawk_feather;
            break;
        case EGG:
            projectile.texture = &assets.texture_projectile_hawk_egg;
            projectile.on_hit = hawk_projectile_explode;
            break;
    }

    // ReSharper disable once CppSomeObjectMembersMightNotBeInitialized
    ((struct Projectile*)projectiles->data)[size] = projectile;

    projectiles->size = size + 1;

    return &((struct Projectile*)projectiles->data)[size];
}

void hawk_projectile_draw(struct Projectile *projectile) {
    struct HawkProjectileData *data = projectile->data;

    switch (data->type) {
        case BIRD: {
            const int sprite = (int) floorf(projectile->lifespan * 10.0f) % 8;
            const int width = projectile->texture->width / 8;

            DrawTexturePro(
                *projectile->texture,
                (Rectangle){(float) sprite * (float) width, 0, (float) (data->is_right_spawn ? -width : width), (float) projectile->texture->height},
                (Rectangle){projectile->position.x, projectile->position.y, (float) width, (float) projectile->texture->height},

                (Vector2){(float) width / 2.0f, (float) projectile->texture->height / 2.0f},
                data->rotation,
                WHITE
            );
            break;
        }
        case FEATHER:
            DrawTexturePro(
                *projectile->texture,
                (Rectangle){0, 0, (float) projectile->texture->width, (float) projectile->texture->height},
                (Rectangle){projectile->position.x, projectile->position.y, (float) projectile->texture->width, (float) projectile->texture->height},

                (Vector2){(float) projectile->texture->width / 2.0f, (float) projectile->texture->height / 2.0f},
                data->rotation,
                WHITE
            );
            break;
        case EGG: {
            int sprite;
            int width;
            if (projectile->texture == &assets.texture_projectile_hawk_egg) {
                sprite = 0;
                width = projectile->texture->width;
            } else {
                sprite = (int) floorf(projectile->lifespan * 10.0f) % 5;
                width = projectile->texture->width / 5;
            }


            DrawTexturePro(
                *projectile->texture,
                (Rectangle){(float) sprite * (float) width, 0, (float) width, (float) projectile->texture->height},
                (Rectangle){projectile->position.x, projectile->position.y, (float) width, (float) projectile->texture->height},

                (Vector2){(float) width / 2.0f, (float) projectile->texture->height / 2.0f},
                data->rotation,
                WHITE
            );
            break;
        }
    }

    // Rectangle rectangle = hawk_projectile_hitbox(projectile);
    // DrawRectangleRec(rectangle, Fade(RED, 0.5f));
}

Rectangle hawk_projectile_hitbox(struct Projectile *projectile) {
    struct HawkProjectileData *data = projectile->data;

    switch (data->type) {
        case BIRD: {
            const int sprite = (int) floorf(projectile->lifespan * 10.0f) % 8;
            const float width = (float) projectile->texture->width / 8.0f;

            Rectangle hitbox;

            if (sprite == 0) {
                hitbox = (Rectangle){
                    projectile->position.x - width / 2,
                    projectile->position.y - projectile->texture->height / 2,
                    width * 0.73f,
                    projectile->texture->height * 0.61f
                };
            } else if (sprite == 1) {
                hitbox = (Rectangle){
                    projectile->position.x - width / 2 + (width * 0.04f),
                    projectile->position.y - projectile->texture->height / 2,
                    width * 0.70f,
                    projectile->texture->height * 0.61f
                };
            } else if (sprite == 2) {
                hitbox = (Rectangle){
                    projectile->position.x - width / 2 + (width * 0.04f),
                    projectile->position.y - projectile->texture->height / 2 + (projectile->texture->height * 0.05f),
                    width * 0.72f,
                    projectile->texture->height * 0.55f
                };
            } else if (sprite == 3) {
                hitbox = (Rectangle){
                    projectile->position.x - width / 2 + (width * 0.11f),
                    projectile->position.y - projectile->texture->height / 2 + (projectile->texture->height * 0.25f),
                    width * 0.80f,
                    projectile->texture->height * 0.49f
                };
            } else if (sprite == 4) {
                hitbox = (Rectangle){
                    projectile->position.x - width / 2 + (width * 0.11f),
                    projectile->position.y - projectile->texture->height / 2 + (projectile->texture->height * 0.32f),
                    width * 0.88f,
                    projectile->texture->height * 0.55f
                };
            } else if (sprite == 5) {
                hitbox = (Rectangle){
                    projectile->position.x - width / 2 + (width * 0.12f),
                    projectile->position.y - projectile->texture->height / 2 + (projectile->texture->height * 0.32f),
                    width * 0.82f,
                    projectile->texture->height * 0.67f
                };
            } else if (sprite == 6) {
                hitbox = (Rectangle){
                    projectile->position.x - width / 2 + (width * 0.12f),
                    projectile->position.y - projectile->texture->height / 2 + (projectile->texture->height * 0.32f),
                    width * 0.77f,
                    projectile->texture->height * 0.57f
                };
            } else if (sprite == 7) {
                hitbox = (Rectangle){
                    projectile->position.x - width / 2 + (width * 0.11f),
                    projectile->position.y - projectile->texture->height / 2 + (projectile->texture->height * 0.18f),
                    width * 0.62f,
                    projectile->texture->height * 0.49f
                };
            }

            if (data->is_right_spawn) {
                float offset_x = hitbox.x - (projectile->position.x - width / 2.0f);
                hitbox.x = projectile->position.x + width / 2.0f - offset_x - hitbox.width;
            }

            return hitbox;
        }
            
        case FEATHER: {
            // AABB collision
            // new_hw = |hw·cos θ| + |hh·sin θ|
            // new_hh = |hw·sin θ| + |hh·cos θ|

            const float half_width = (float) projectile->texture->width  / 2.0f;
            const float half_height = (float) projectile->texture->height / 2.0f;

            const float angle_rad = data->rotation * DEG2RAD;
            const float cos = fabsf(cosf(angle_rad));
            const float sin = fabsf(sinf(angle_rad));

            const float new_half_width = half_width * cos + half_height * sin;
            const float new_half_height = half_width * sin + half_height * cos;

            return (Rectangle){
                projectile->position.x - new_half_width,
                projectile->position.y - new_half_height,
                new_half_width * 2.0f,
                new_half_height * 2.0f
            };
        }

        case EGG:
            if (projectile->texture == &assets.texture_projectile_explosion) {
                const float width = (float) projectile->texture->width / 5.0f;

                return (Rectangle){
                    projectile->position.x - width / 2,
                    projectile->position.y - (float) projectile->texture->height / 2,
                    width,
                    (float) projectile->texture->height
                };
            }

            return (Rectangle){
                projectile->position.x - projectile->texture->width / 2 + 1,
                projectile->position.y - projectile->texture->height / 2 + 1,
                projectile->texture->width - 2,
                projectile->texture->height - 2
            };
    }

    return (Rectangle){0, 0, 0, 0};
}


bool hawk_attack_1(struct Array *projectiles, float timer, bool first_tick) {
    float frame_time = GetFrameTime();

    // x projectiles per second
    if (first_tick || hawk_attack_interval >= 1.0f / 2.5f) {
        if (!first_tick)
            hawk_attack_interval -= 1.0f / 2.5f;

        bool is_right_spawn = rand() & 1;

        Texture2D *texture = &assets.texture_projectile_hawk_bird;
        const float width = (float) texture->width / 8.0f;

        const float x = is_right_spawn ? BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + width : BATTLE_BOUNDS.x - width;
        const float y = Lerp(BATTLE_BOUNDS.y + (BATTLE_BOUNDS.height / 4.0f), BATTLE_BOUNDS.y + (BATTLE_BOUNDS.height / 4.0 * 3.0f), (float) ((double) rand() / (double) RAND_MAX));

        struct Projectile *projectile = spawn_hawk_projectile(projectiles, (int) x, (int) y, BIRD);

        struct HawkProjectileData *data = projectile->data;
        data->velocity = (Vector2){is_right_spawn ? -1.0f : 1.0f, 0};
        data->is_right_spawn = is_right_spawn;

        projectile->lifespan = (float) ((double) rand() / (double) RAND_MAX) * PI * 2.0f;
    }

    hawk_attack_interval += frame_time;

    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct HawkProjectileData *data = projectile->data;
        const float speed = 128.0f * frame_time;

        projectile->position.x += data->velocity.x * speed;

        projectile->position.y += sinf(projectile->lifespan * PI) * 1.65f;

        if (data->is_right_spawn && projectile->position.x < BATTLE_BOUNDS.x - (float) projectile->texture->width / 8.0f) {
            projectile_despawn(projectile);
        } else if (!data->is_right_spawn && projectile->position.x > BATTLE_BOUNDS.x + BATTLE_BOUNDS.width + (float) projectile->texture->width / 8.0f) {
            projectile_despawn(projectile);
        }
    }

    return timer >= 6.0f;
}

bool hawk_attack_2(struct Array *projectiles, float timer, bool first_tick) {
    float frame_time = GetFrameTime();

    // x projectiles per second
    if (first_tick || hawk_attack_interval >= 1.0f / 5.2f) {
        if (!first_tick)
            hawk_attack_interval -= 1.0f / 5.2f;


        const float x = Lerp(BATTLE_BOUNDS.x, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width, (float) ((double) rand() / (double) RAND_MAX));
        const float y = Lerp(BATTLE_BOUNDS.y - (BATTLE_BOUNDS.height / 2.0f), BATTLE_BOUNDS.y - (BATTLE_BOUNDS.height / 4.0), (float) ((double) rand() / (double) RAND_MAX));

        struct Projectile *projectile = spawn_hawk_projectile(projectiles, (int) x, (int) y, EGG);

        struct HawkProjectileData *data = projectile->data;
        data->velocity = (Vector2){1};
        data->target_y = Lerp(BATTLE_BOUNDS.y, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height - (float) assets.texture_projectile_wizard_fireball.height, (float) pow((double) rand() / (double) RAND_MAX, 0.3));
    }

    hawk_attack_interval += frame_time;

    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];
        const struct HawkProjectileData *data = projectile->data;

        if (projectile->texture == &assets.texture_projectile_explosion) {
            if (projectile->lifespan * 8.0f >= 5.0f) {
                projectile_despawn(projectile);
            }
        } else {
            if (projectile->position.y > data->target_y) {
                hawk_projectile_explode(projectile);

            } else {
                projectile->position.y += Clamp(projectile->lifespan * 96.0f, 0, 256.0f) * frame_time;
            }
        }
    }

    return timer > 5.5f;
}

void hawk_projectile_explode(struct Projectile *projectile) {
    projectile->texture = &assets.texture_projectile_explosion;
    projectile->lifespan = 0;
    projectile->on_hit = projectile_damage_player;

    PlaySound(assets.sfx_explosion);
}

float hawk_attack_3_interval;
bool hawk_attack_3(struct Array *projectiles, float timer, bool first_tick) {
    float frame_time = GetFrameTime();

    // x projectiles per second
    if (first_tick || hawk_attack_interval >= 1.0f / 3.0f) {
        if (!first_tick)
            hawk_attack_interval -= 1.0f / 3.0f;

        bool is_left_spawn = rand() & 1;
        bool is_up_spawn = rand() & 1;

        Texture2D *texture = &assets.texture_projectile_hawk_feather;
        const float width = (float) texture->width / 8.0f;

        float x;
        float y;

        if (is_left_spawn) {
            x = Lerp(0, SCREEN_WIDTH, (float) ((double) rand() / (double) RAND_MAX));
            y = is_up_spawn ? 0 : SCREEN_HEIGHT;
        } else {
            x = is_up_spawn ? 0 : SCREEN_WIDTH;
            y = Lerp(0, SCREEN_HEIGHT, (float) ((double) rand() / (double) RAND_MAX));
        }

        struct Projectile *projectile = spawn_hawk_projectile(projectiles, (int) x, (int) y, FEATHER);
        struct HawkProjectileData *data = projectile->data;

        const float dest_x = Lerp(BATTLE_BOUNDS.x, BATTLE_BOUNDS.x + BATTLE_BOUNDS.width, (float) ((double) rand() / (double) RAND_MAX));
        const float dest_y = Lerp(BATTLE_BOUNDS.y, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height - (float) assets.texture_projectile_wizard_fireball.height, (float) pow((double) rand() / (double) RAND_MAX, 0.3));
        const float angle = atan2f(x - dest_x, dest_y - y) + 90 * DEG2RAD;

        data->rotation = angle * RAD2DEG + 90;
        data->velocity = (Vector2){cosf(angle), sinf(angle)};
    }

    if (hawk_attack_3_interval > 1.0f / 15.0f && timer < 3.6f) {
        hawk_attack_3_interval -= 1.0f / 15.0f;
        hawk_attack_index++;

        const float y = 16 + 4 + (float) assets.texture_enemy_golem.height / 2.0f;
        const float speed = 0.5f + timer / 2.0f;

        for (int i = 0; i < 2; i++) {
            float x = BATTLE_BOUNDS.x - (BATTLE_BOUNDS.width / 2.0f);
            if (i == 1) {
                x += BATTLE_BOUNDS.width * 2.0f;
            }
            const float rotation = fmodf(
                        180.0f + ((float) hawk_attack_index * 20.0f) + (float)((double) rand() / (double) RAND_MAX * 40.0),
                        360.0f
                    );
            const float deg2rad_rotation = DEG2RAD * rotation;

            struct Projectile *projectile = spawn_hawk_projectile(projectiles, (int) x, (int) y, FEATHER);
            struct HawkProjectileData *data = projectile->data;

            data->velocity = (Vector2){cosf(deg2rad_rotation) * speed, sinf(deg2rad_rotation) * speed};
            data->rotation = rotation + 90;
        }
    }

    hawk_attack_interval += frame_time;
    hawk_attack_3_interval += frame_time;

    const int size = projectiles->size;
    for (int i = 0; i < size; i++) {
        struct Projectile* projectile = &((struct Projectile*)projectiles->data)[i];

        struct HawkProjectileData *data = projectile->data;
        const float speed = 128.0f * frame_time;

        projectile->position.x += data->velocity.x * speed;
        projectile->position.y += data->velocity.y * speed;
    }

    if (timer >= 6.0f) {
        hawk_attack_3_interval = 0;
        return true;
    }

    return false;
}