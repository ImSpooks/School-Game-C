#include "scene/scene_battle.h"

#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "player/player.h"
#include "util/util.h"
#include "asset_manager.h"
#include "enemy/projectile/projectile.h"

void battle_attack(struct Scene *scene, struct Hud *hud);
void end_battle(struct BattleData *data, struct Hud *hud);
void end_battle_button(struct Scene *scene, struct Hud *hud);

int music_index;

void battle_scene_load(struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    data->enemy.health = data->enemy.max_health;

    data->state = PLAYER_TURN;
    data->turn = 0;
    data->context.timer = 0;

    hud_set_dialogue(hud, "");

    music_index = 0;

    data->projectiles = array_empty();


    array_allocate(&hud->buttons, sizeof(struct Button), 1);
    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Attack",
        .on_click = battle_attack,
    };
    hud->buttons.size = 1;

    data->enemy.initialize(&data->enemy);

    const int size = data->enemy.music.size;
    if (size > 0) {
        Music* music = ((Music**) data->enemy.music.data)[0];
        PlayMusicStream(*music);
        scene->music = music;

        for (int i = 0; i < size; i++) {
            SetMusicVolume(*((Music**) data->enemy.music.data)[i], 1.0f);
        }
    }

    hud_unset_flag(hud, RENDER_ITEMS);
    hud_set_flag(hud, RENDER_HEALTH);
}

void battle_scene_unload(struct Scene *scene, struct Hud *hud){
    struct BattleData *data = scene->data;

    if (data->enemy.unload != NULL) {
        data->enemy.unload(&data->enemy);
    }

    if (data->state == ENEMY) {
        // Should only happen on force close
        const int size = data->projectiles.size;
        struct Projectile* projectiles = data->projectiles.data;
        for (int i = 0; i < size; i++) {
            free(projectiles[i].data);
        }
    }

    // Stop all musics from playing
    const int size = data->enemy.music.size;
    for (int i = 0; i < size; i++) {
        Music* music = ((Music**) data->enemy.music.data)[i];
        if (IsMusicStreamPlaying(*music)) {
            StopMusicStream(*music);
        }
    }

    array_free(&data->projectiles);

    hud_set_flag(hud, RENDER_ITEMS);
    hud_unset_flag(hud, RENDER_HEALTH);
}

void battle_scene_update(struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    const int music_size = data->enemy.music.size;
    if (music_size > 0) {
        Music* active_music = ((Music**) data->enemy.music.data)[music_index];
        if (!IsMusicStreamPlaying(*active_music)) {
            StopMusicStream(*active_music);

            music_index++;
            if (music_index >= music_size) {
                music_index = 0;
            }

            active_music = ((Music**) data->enemy.music.data)[music_index];
            PlayMusicStream(*active_music);
            scene->music = active_music;
        }
    }

    float frame_time = GetFrameTime();

    if (data->state == CALCULATING_DAMAGE) {
        if (data->context.timer >= CALCULATING_DAMAGE_LENGTH) {
            data->context.timer = 0;
            data->state = DEALING_DAMAGE;

            PlaySound(assets.sfx_attack);
        }
    } else if (data->state == DEALING_DAMAGE) {
        if (data->context.timer >= DEALING_DAMAGE_LENGTH) {
            data->context.timer = 0;
            data->state = MOVE_HEALTHBAR;

            PlaySound(assets.sfx_damage_enemy);
        }
    } else if (data->state == MOVE_HEALTHBAR) {
        if (data->context.timer >= RENDERING_DAMAGE_LENGTH) {
            data->context.timer = 0;
            data->state = ENEMY;

            data->enemy.health -= data->context.last_damage;
            data->context.last_damage = floor((double)rand() / (double) RAND_MAX * data->enemy.total_attacks);

            player.position.x = BATTLE_BOUNDS_MIDDLE.x;
            player.position.y = BATTLE_BOUNDS_MIDDLE.y;
            player.invincibility_timer = 0;

        }
    } else if (data->state == ENEMY) {
        if (data->enemy.health <= 0) {
            data->state = FINISHED;
            end_battle(data, hud);

        } else if (data->context.timer > BATTLE_BOUNDS_EXPAND_TIME) {
            if (player.invincibility_timer > 0) {
                player.invincibility_timer -= frame_time;
            }

            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
                player.position.y = player.position.y - (PLAYER_SPEED * frame_time);
            } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
                player.position.y = player.position.y + (PLAYER_SPEED * frame_time);
            }

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                player.position.x = player.position.x - (PLAYER_SPEED * frame_time);
            } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                player.position.x = player.position.x + (PLAYER_SPEED * frame_time);
            }

            player.position.x = Clamp(player.position.x, BATTLE_BOUNDS.x + 8 + 3,
                                      BATTLE_BOUNDS.x + BATTLE_BOUNDS.width - 8 - 3);
            player.position.y = Clamp(player.position.y, BATTLE_BOUNDS.y + 8 + 3, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height - 8 - 3);

            if (data->enemy.attack(&data->projectiles, data->context.enemy_rand_attack, data->context.timer - BATTLE_BOUNDS_EXPAND_TIME, data->turn)) {
                data->context.timer = 0;
                data->state = ENEMY_FINISH;
                data->turn++;

                hud->buttons.size = 1;

                const int size = data->projectiles.size;
                struct Projectile* projectiles = data->projectiles.data;
                for (int i = 0; i < size; i++) {
                    free(projectiles[i].data);
                }
            } else {
                Rectangle player_hitbox = (Rectangle) {player.position.x - 8, player.position.y - 8, 16, 16};

                int size = data->projectiles.size;
                struct Projectile* projectiles = data->projectiles.data;
                for (int i = 0; i < size; i++) {
                    struct Projectile* projectile = &projectiles[i];

                    Rectangle hitbox = projectile->hitbox(projectile);

                    // Remove projectiles outside of bounds
                    if (!CheckCollisionRecs(hitbox, (Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}) || projectile->lifespan > 99) {
                        free(projectile->data);
                        *projectile = projectiles[--size];
                    }

                    if (player.invincibility_timer <= 0) {
                        if (CheckCollisionRecs(player_hitbox, hitbox)) {
                            projectile_damage_player(projectile, projectile->true_damage);
                            PlaySound(assets.sfx_damage_take);
                        }
                    }

                    projectile->lifespan += frame_time;
                }
                if (size != data->projectiles.size)
                    data->projectiles.size = size;
            }
        }
    } else if (data->state == ENEMY_FINISH) {
        if (data->context.timer >= BATTLE_BOUNDS_EXPAND_TIME) {
            data->context.timer = 0;
            data->state = PLAYER_TURN;
        }
    }

    data->context.timer += frame_time;


}

void battle_scene_draw(void* scene_data) {
    struct BattleData *data = scene_data;
    ClearBackground(BLACK);

    for (int x = 0; x < 640; x += 64) {
        DrawLine(x, 0, x, 360, GREEN);
    }

    for (int y = 0; y < 360; y += 60) {
        DrawLine(0, y, 640, y, GREEN);
    }

    int y = 64 - (*data->enemy.texture).height / 2;
    if (y < 16) y = 16;

    if (data->enemy.health > 0) {
        Vector2 pos = {640 / 2.0f - (float)(*data->enemy.texture).width / 2.0f, y};
        DrawTexture(*data->enemy.texture, (int)pos.x, (int)pos.y, WHITE);
    }

    if (data->state == CALCULATING_DAMAGE) {
        DrawTexture(assets.texture_ui_damage_bar, 640 / 2 - assets.texture_ui_damage_bar.width / 2, 360 - 162, WHITE);

        int startPos = 640 / 2 - assets.texture_ui_damage_bar.width / 2;
        int endPos   = 640 / 2 + assets.texture_ui_damage_bar.width / 2;
        float percent = data->context.timer / CALCULATING_DAMAGE_LENGTH;
        int pos = (int) ((float) startPos + (float) (endPos - startPos) * percent);

        DrawTexture(assets.texture_ui_damage_slider, pos - assets.texture_ui_damage_slider.width / 2, 360 - 162 + assets.texture_ui_damage_bar.height / 2 - assets.texture_ui_damage_slider.height / 2, WHITE);


    } else if (data->state == DEALING_DAMAGE) {
        int sprite = (int)floorf((data->context.timer / DEALING_DAMAGE_LENGTH) * 6);
        int width = assets.texture_ui_damage_slice.width / 6;

        DrawTextureRec(assets.texture_ui_damage_slice, (Rectangle) {(float) (sprite * width), 0, (float) width, (float) assets.texture_ui_damage_slice.height},
            (Vector2){640.0f / 2.0f - (float)width / 2.0f, ((float) y + (float) (*data->enemy.texture).height / 2.0f) - (float) assets.texture_ui_damage_slice.height / 2.0f}, WHITE);
    } else if (data->state == MOVE_HEALTHBAR){
        const char* text = TextFormat("%d", (int) ceilf(data->context.last_damage));
        int font_size = 25;
        int width = MeasureText(text, font_size);

        Vector2 pos = {(SCREEN_WIDTH / 2.0f) - ((float) width / 2.0f), (float) y + (float) (*data->enemy.texture).height / 4.0f * 3.0f};

        DrawOutlinedText(text,
            (int) (pos.x + ((double) rand() / (double) RAND_MAX - 0.5) * 4),
            (int) ((pos.y - (float) font_size / 2.0f) + ((double) rand() / (double) RAND_MAX - 0.5) * 4),
            font_size, RED, 3, BLACK
        );

        const float maxHealth = data->enemy.max_health;
        const float currentHealth = data->enemy.health;

        const float renderHealth = currentHealth - (data->context.last_damage * (data->context.timer / RENDERING_DAMAGE_LENGTH));

        const float healthBarWidth = 100;
        float renderWitdh = healthBarWidth * (renderHealth / maxHealth);
        if (renderWitdh < 0)
            renderWitdh = 0;

        DrawRectangleRec((Rectangle) {SCREEN_WIDTH / 2 - healthBarWidth / 2, pos.y + font_size + 2, (int)healthBarWidth, 12}, DARKGRAY);
        DrawRectangleRec((Rectangle) {SCREEN_WIDTH / 2 - healthBarWidth / 2, pos.y + font_size + 2, (int)renderWitdh, 12}, RED);
        DrawRectangleLinesEx((Rectangle) {(SCREEN_WIDTH / 2) - healthBarWidth / 2, pos.y + font_size + 2, healthBarWidth, 12}, 1, BLACK);

    } else if (data->state == ENEMY) {
        const int line_width = 3;
        if (data->context.timer < BATTLE_BOUNDS_EXPAND_TIME) {
            const float percent = data->context.timer / BATTLE_BOUNDS_EXPAND_TIME;

            const float expansionX = BATTLE_BOUNDS.width / 2 * percent;
            const float expansionY = BATTLE_BOUNDS.height / 2 * percent;

            Rectangle expandedRect = {
                BATTLE_BOUNDS_MIDDLE.x - expansionX, BATTLE_BOUNDS_MIDDLE.y - expansionY, expansionX * 2, expansionY * 2
            };

            DrawRectangleRec(expandedRect, BLACK);
            DrawRectangleLinesEx(expandedRect, line_width, WHITE);
        } else {
            DrawRectangleRec(BATTLE_BOUNDS, BLACK);
            DrawRectangleLinesEx(BATTLE_BOUNDS, line_width, WHITE);

            // render player
            int width = assets.texture_ui_soul.width / 2;
            int sprite = 0;
            if (player.invincibility_timer > 0) {
                sprite = 1;
            }

            DrawTextureRec(assets.texture_ui_soul, (Rectangle) {sprite * width, 0, width, assets.texture_ui_soul.height},
                (Vector2){player.position.x - width / 2, player.position.y - assets.texture_ui_soul.height / 2}, WHITE);


            const int size = data->projectiles.size;
            struct Projectile* projectiles = data->projectiles.data;
            for (int i = 0; i < size; i++) {
                projectiles[i].draw(&projectiles[i]);
            }
        }
    } else if (data->state == ENEMY_FINISH) {
        const int line_width = 3;
        const float percent = 1 - (data->context.timer / BATTLE_BOUNDS_EXPAND_TIME);

        const float expansionX = BATTLE_BOUNDS.width / 2 * percent;
        const float expansionY = BATTLE_BOUNDS.height / 2 * percent;

        Rectangle expandedRect = {
            BATTLE_BOUNDS_MIDDLE.x - expansionX, BATTLE_BOUNDS_MIDDLE.y - expansionY, expansionX * 2, expansionY * 2
        };

        DrawRectangleRec(expandedRect, BLACK);
        DrawRectangleLinesEx(expandedRect, line_width, WHITE);
    }
}


void end_battle(struct BattleData *data, struct Hud *hud) {
    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Continue",
        .on_click = end_battle_button,
    };
    hud->buttons.size = 1;

    if (data->enemy.pre_defeat != NULL)
        data->enemy.pre_defeat();

    const int size = data->enemy.music.size;
    for (int i = 0; i < size; i++) {
        SetMusicVolume(*((Music**) data->enemy.music.data)[i], 0.4f);
    }

    PlaySound(assets.sfx_victory);
}

void battle_attack(struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    if (data->state == PLAYER_TURN) {
        hud_hide_dialogue(hud);
        data->state = CALCULATING_DAMAGE;
        data->context.timer = 0;
        data->context.last_damage = 0;
    } else if (data->state == CALCULATING_DAMAGE) {

        float percent = data->context.timer / CALCULATING_DAMAGE_LENGTH; // 0 to 1 range, 0.5 is optimal damage
        float damage = sinf(percent * PI) * 15;

        damage = damage * player_get_attack(player) / sqrtf(powf((float)data->enemy.defence_stat, 1.75f));
        damage = damage * (powf((float)data->turn / 10.0f, 2) + 1);

        data->context.last_damage = damage * 1;

        data->context.timer = 0;
        data->state = DEALING_DAMAGE;

        hud->buttons.size = 0;
        PlaySound(assets.sfx_attack);
    }
}

void end_battle_button(struct Scene *scene, struct Hud *hud) {
    hud->buttons.size = 0;

    if (player.flags.boss_hawk) {

    } else {
        scene->request_scene_change.type = LEVEL;
    }
}

void battle_scene_post_unload(struct Scene *scene, struct Hud *hud) {
    struct BattleData *data = scene->data;

    if (data->enemy.post_defeat != NULL)
        data->enemy.post_defeat(hud);
}