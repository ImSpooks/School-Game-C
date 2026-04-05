#include "scene/scene_gameover.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <raymath.h>

#include "asset_manager.h"
#include "globals.h"
#include "enemy/projectile/projectile.h"

// Letters per second for the typewriter effect
#define GAMEOVER_LETTERS_PER_SEC 12.5f
// Y position for the ASGORE text lines
#define GAMEOVER_TEXT_FONT  16

void button_restart(struct Scene *scene, struct Hud *hud);

void gameover_scene_load(struct Scene *scene, struct Hud *hud) {
    struct GameOverData *data = scene->data;
    data->state = INIT;
    data->timer = 0.0f;
    data->letters = 0.0f;
    data->active_line = 0;
    data->wait_timer = 0.0f;

    array_allocate(&data->soul_particles, sizeof(Vector4), 6);

    hud_unset_flag(hud, RENDER_ITEMS);

    array_allocate(&hud->buttons, sizeof(struct Button), 1);

    ((struct Button*)hud->buttons.data)[0] = (struct Button) {
        .text = "Restart",
        .on_click = button_restart
    };

    // dont render yet
    hud->buttons.size = 0;
    hud_hide_dialogue(hud);
}

void gameover_scene_unload(struct Scene *scene, struct Hud *hud) {
    struct GameOverData *data = scene->data;
    array_free(&data->soul_particles);
}

void gameover_scene_update(struct Scene *scene, struct Hud *hud) {
    float frame_time = GetFrameTime();

    struct GameOverData *data = scene->data;

    if (data->wait_timer >= 0) {
        data->wait_timer -= frame_time;
        return;
    }

    data->timer += frame_time;

    if (data->state == INIT && data->timer >= 0.5f) {
        data->state = SOUL_BREAK;
        data->timer = 0.0f;

        PlaySound(assets.sfx_soul_gameover_hit);
    } else if (data->state == SOUL_BREAK && data->timer >= 0.8f) {
        data->state = SOUL_SHATTER;
        data->timer = 0.0f;

        PlaySound(assets.sfx_soul_gameover_break);

        for (int i = 0; i < 6; i++) {
            Vector4 position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 5.0f * 4,
                -7.5f + (float)((double) rand() / (double) RAND_MAX) * 15.0f,
                -15.5f + (float)((double) rand() / (double) RAND_MAX) * 17.5f
            };

            ((Vector4*)data->soul_particles.data)[i] = position;
        }
        data->soul_particles.size = 6;
    } else if (data->state == SOUL_SHATTER && data->timer >= 1.5f) {
        data->state = ASGORE;
        data->timer = 0.0f;

        scene->music = &assets.music_gameover;
        if (!IsMusicStreamPlaying(*scene->music)) {
            PlayMusicStream(*scene->music);
        }
        data->wait_timer = 0.5f;
    } else if (data->state == ASGORE) {
        const char* lines[2] = { GAME_OVER_TEXT_1, GAME_OVER_TEXT_2 };

        if (data->active_line < 2) {
            if (data->queue_next_line) {
                data->active_line++;
                data->letters = 0.0f;
                data->queue_next_line = false;
            }

            const char* text = lines[data->active_line];
            int text_len = (int)strlen(text);

            int old_letters = (int)floorf(data->letters);

            data->letters += frame_time * GAMEOVER_LETTERS_PER_SEC;

            if ((int)floorf(data->letters) != old_letters) {
                PlaySound(assets.sfx_death_voice);
            }

            char character = text[(int)floorf(data->letters)];

            if (character == '%') {
                data->letters = floorf(data->letters) + 1.0f; // hop to next letter
                data->wait_timer = 0.5f;
            } else if (character == '\n') {
                data->letters = data->letters + 1.0f;
            }

            if (data->letters >= (float)text_len) {
                data->letters = (float) text_len;

                if (data->active_line == 0) {
                    data->wait_timer = 2.0f;
                    data->queue_next_line = true;
                } else {
                    hud->buttons.size = 1;
                }
            }
        }
    }
}

void gameover_scene_draw(void *scene_data) {
    struct GameOverData *data = scene_data;
    float frame_time = GetFrameTime();

    ClearBackground(BLACK);

    Vector2 position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 5.0f * 4};

    if (data->state == INIT) {

        DrawTexturePro(
            assets.texture_ui_soul,
            (Rectangle){0, 0, (float) assets.texture_ui_soul.width / 2.0f, (float) assets.texture_ui_soul.height},
            (Rectangle){position.x, position.y, (float) assets.texture_ui_soul.width / 2.0f, (float) assets.texture_ui_soul.height},

            (Vector2){(float)  assets.texture_ui_soul.width / 2.0f / 2.0f, (float)  assets.texture_ui_soul.height / 2.0f},
            0,
            WHITE
        );
    } else if (data->state == SOUL_BREAK) {
        DrawTexturePro(
            assets.texture_ui_soul_broken,
            (Rectangle){0, 0, (float) assets.texture_ui_soul_broken.width, (float) assets.texture_ui_soul_broken.height},
            (Rectangle){position.x, position.y, (float) assets.texture_ui_soul_broken.width, (float) assets.texture_ui_soul_broken.height},

            (Vector2){(float)  assets.texture_ui_soul_broken.width / 2.0f, (float)  assets.texture_ui_soul_broken.height / 2.0f},
            0,
            WHITE
        );
    } else if (data->state == SOUL_SHATTER || data->state == ASGORE) {
        for (int i = 0; i < data->soul_particles.size; i++) {
            Vector4 *particle = &((Vector4*)data->soul_particles.data)[i];

            particle->x += particle->z * GetFrameTime() * 32.0f;
            particle->y += particle->w * GetFrameTime() * 32.0f;

            // Old game was x * 0.9975 per 20ms (50 ticks/s), 0.9975^50 = 0.8824 units/s
            particle->z = particle->z * powf(0.8824f, frame_time);
            // Old game was y - 0.2 per 20ms (50 ticks/s), 0.2 * 50 = 10 units/s
            particle->w += 10.0f * frame_time;

            float width = (float) assets.texture_ui_soul_pieces.width / 4.0f;
            int sprite = (int) floorf(data->timer * 50) % 4;

            DrawTexturePro(
                assets.texture_ui_soul_pieces,
                (Rectangle){(float) sprite * width, 0, width, (float) assets.texture_ui_soul_pieces.height},
                (Rectangle){particle->x, particle->y, width, (float) assets.texture_ui_soul_pieces.height},

                (Vector2){width / 2.0f, (float)  assets.texture_ui_soul_pieces.height / 2.0f},
                0,
                WHITE
            );
        }
    }

    if (data->state == ASGORE) {

        int game_over_width = MeasureText("GAME OVER", 60);
        float alpha = Clamp(data->timer / 3.0f, 0, 1);
        DrawText("GAME OVER", SCREEN_WIDTH / 2 - game_over_width / 2, SCREEN_HEIGHT / 5.0f, 60, Fade(WHITE, alpha));


        const char* lines[2] = { GAME_OVER_TEXT_1, GAME_OVER_TEXT_2 };
        const char* text = lines[data->active_line];


        int text_len = (int)strlen(text);

        int line_count = 1;

        for (int i = 0; i < text_len; i++) {
            if (text[i] == '\n') {
                line_count++;
            }
        }

        // assume text_len for this case since we don't render a book
        char row_buffer[line_count][text_len];
        memset(row_buffer, 0, sizeof(row_buffer));

        int letters = (int)floorf(data->letters);

        int line_index = 0;
        int row_position = 0;

        for (int i = 0; i < letters; i++) {
            char c = text[i];
            if (c == '%') {
                // do nothing
            } else if (c == '\n') {
                line_index++;
                row_position = 0;
            } else {
                if (row_position < text_len - 1) {
                    row_buffer[line_index][row_position++] = c;
                }
            }
        }

        for (int i = 0; i < line_count; i++) {
            int width = MeasureText(row_buffer[i], GAMEOVER_TEXT_FONT);

            int x = SCREEN_WIDTH / 2 - width / 2;
            int y = (int) (SCREEN_HEIGHT / 5.0f * 4.0f) + i * (GAMEOVER_TEXT_FONT + 4);

            DrawText(row_buffer[i], x, y, GAMEOVER_TEXT_FONT, WHITE);
        }
    }
}

void button_restart(struct Scene *scene, struct Hud *hud) {
    reset_game();
    scene->request_scene_change.type = TITLE;
}