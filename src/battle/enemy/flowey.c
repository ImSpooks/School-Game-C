#include "flowey.h"

#include <math.h>
#include <stdio.h>
#include <raymath.h>

#include "../../globals.h"
#include "../../items/items.h"
#include "enemy.h"
#include "vec.h"
#include "../../player/player.h"
#include "../../screen/screen.h"
#include "../../asset_manager.h"

Texture2D** enemy_flowey_projectile_texture;

void spawn_flowey_projectile(Projectile **projectiles, int x, int y);
void flowey_projectile_draw(Projectile *projectile);
Rectangle flowey_projectile_hitbox(Projectile *projectile);

void enemy_flowey_initialize(Enemy *enemy) {
    enemy->texture = &assets.texture_enemy_flowey;

    enemy->music = vector_create();
    vector_add(&enemy->music, &assets.music_flowey);
}

void enemy_flowey_unload(Enemy *enemy) {
    vector_free(enemy->music);
}

bool enemy_flowey_attack(Projectile **projectiles, int type, float timer, int turn) {
    const int waves = Clamp(turn + 1, 1, 4);

    // Spawn projectiles nicely in an arc
    if (timer < 0.05) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2) - 64;
        const int y = 48;

        while (vector_size(*projectiles) < waves) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    } else if (timer < 0.1) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2) - 32;
        const int y = 32;

        while (vector_size(*projectiles) < waves * 2) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    } else if (timer < 0.15) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2);
        const int y = 16;

        while (vector_size(*projectiles) < waves * 3) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    } else if (timer < 0.2) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2) + 32;
        const int y = 32;

        while (vector_size(*projectiles) < waves * 4) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    } else if (timer < 0.25) {
        const int x = SCREEN_WIDTH / 2 - (assets.texture_projectile_flowey.width / 2 / 2) + 64;
        const int y = 48;

        while (vector_size(*projectiles) < waves * 5) {
            spawn_flowey_projectile(projectiles, x, y);
        }
    }


    size_t size = vector_size(*projectiles);
    for (int i = 0; i < size; i++) {
        Projectile *projectile = *projectiles + i;

        const float delay = floorf(i % waves) * 0.2f + 0.85f;

        if (timer > delay) {
            Vector2 *velocity = projectile->data;

            if (velocity->x == 0 && velocity->y == 0) {

                Vector2 vec = { player.position.x - (projectile->position.x + 8), player.position.y - (projectile->position.y + 8) };
                vec = Vector2Normalize(vec);

                velocity->x = vec.x;
                velocity->y = vec.y;
            }


            const float speed = Clamp((timer - delay) / 2.5f, 0, 0.8f);

            projectile->position.x += velocity->x * speed;
            projectile->position.y += velocity->y * speed;
        }

    }

    return timer > 0.25f && size == 0;
}

void enemy_flowey_pre_defeat() {
    player.flags.boss_flowey = true;

    Item* item = vector_add_dst(&player.inventory);
    item->type = COIN;
    item->texture = &assets.texture_item_coin;
    item->value = 1;
}

void enemy_flowey_post_defeat() {
    setDialogue("Farmer Johan: As promised, I'll hand over the golden coin.");
}

void spawn_flowey_projectile(Projectile **projectiles, int x, int y) {
    Projectile *projectile = vector_add_dst(&*projectiles);

    projectile->texture = &assets.texture_projectile_flowey;
    projectile->position = (Vector2){x, y};
    projectile->damage = 2;
    projectile->true_damage = false;
    projectile->data = malloc(sizeof(Vector2));

    // initialize later
    Vector2* test = (projectile->data);
    test->x = 0;
    test->y = 0;

    projectile->get_hitbox = flowey_projectile_hitbox;
    projectile->draw = flowey_projectile_draw;

}

void flowey_projectile_draw(Projectile *projectile) {
    int sprite = (int)floorf(projectile->lifespan * 20) % 2;
    int width = projectile->texture->width / 2;

    Rectangle rect = {sprite * width, 0, width, projectile->texture->height};

    DrawTextureRec(*projectile->texture, rect, (Vector2){projectile->position.x, projectile->position.y}, WHITE);
}

Rectangle flowey_projectile_hitbox(Projectile *projectile) {
    return (Rectangle){projectile->position.x, projectile->position.y, projectile->texture->width / 2, projectile->texture->height};
}