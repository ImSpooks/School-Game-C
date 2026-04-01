#include <stdlib.h>
#include <raylib.h>
#include "globals.h"

#include "scene/scene.h"
#include "scene/scene_title.h"
#include "scene/scene_level.h"
#include "scene/scene_battle.h"
#include "scene/scene_gameover.h"
#include "scene/scene_credits.h"

#include "hud.h"
#include "util/util.h"
#include "player/player.h"
#include "asset_manager.h"
#include "level/level_registry.h"

void debug(struct BattleData *data);

int main(void) {
    InitWindow(1280, 720, "Adventure Game");
    InitAudioDevice();

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // Initialize game stuff
    assets_load();
    struct Hud hud = {
        .buttons = array_empty(),
        .dialogue = array_empty(),
    };
    hud.option_flags = RENDER_HUD | RENDER_BUTTONS | RENDER_ITEMS;

    level_registry_initialize(&level_registry);

    struct TitleData title_data = {};
    struct Scene title_scene = {
        .load   = title_scene_load,
        .unload = title_scene_unload,
        .update = title_scene_update,
        .draw   = title_scene_draw,

        .data = &title_data,
        .music = &assets.empty_music,
        .hud = &hud,
    };

    struct LevelData level_data = {
        .current_level = &level_registry.levelStart
    };
    struct Scene level_scene = {
        .load   = level_scene_load,
        .unload = level_scene_unload,
        .update = level_scene_update,
        .draw   = level_scene_draw,

        .music = &assets.empty_music,
        .hud = &hud,
        .data = &level_data
    };

    struct BattleData battle_data = {};
    struct Scene battle_scene = {
        .load   = battle_scene_load,
        .unload = battle_scene_unload,
        .post_unload = battle_scene_post_unload,
        .update = battle_scene_update,
        .draw   = battle_scene_draw,

        .music = &assets.empty_music,
        .hud = &hud,
        .data = &battle_data
    };

    struct GameOverData gameover_data = {};
    struct Scene gameover_scene = {
        .load   = gameover_scene_load,
        .unload = gameover_scene_unload,
        .update = gameover_scene_update,
        .draw   = gameover_scene_draw,

        .music = &assets.music_gameover,
        .hud = &hud,
        .data = &gameover_data
    };

    struct CreditsData credits_data = {};
    struct Scene credits_scene = {
        .load   = credits_scene_load,
        .unload = credits_scene_unload,
        .update = credits_scene_update,
        .draw   = credits_scene_draw,

        .music = &assets.music_credits,
        .hud = &hud,
        .data = &credits_data
    };

    player.inventory = array_empty();

    // TODO
    // title_scene.load(&title_scene, &hud);
    // enum SceneType scene = TITLE;

    enum SceneType scene = BATTLE;
    debug(&battle_data);
    battle_scene.load(&battle_scene, &hud);

    int monitor = GetCurrentMonitor();
    SetTargetFPS(GetMonitorRefreshRate(monitor));


    const RenderTexture2D screenRenderer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    long frame = 0;
    while (!WindowShouldClose()) {
        // Update
        if (IsKeyDown(KEY_R) && GetScreenWidth() != 1280 && GetScreenHeight() != 720) {
                SetWindowSize(1280, 720);
        }
        if (IsKeyDown(KEY_O) && GetScreenWidth() != SCREEN_WIDTH && GetScreenHeight() != SCREEN_HEIGHT) {
            SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        }

        if (monitor != GetCurrentMonitor()) {
            monitor = GetCurrentMonitor();
            SetTargetFPS(GetMonitorRefreshRate(monitor));
        }

        struct Scene *current_scene = 0;

        switch (scene) {
            case NONE:
                continue;
            case TITLE:
                current_scene = &title_scene;
                break;
            case LEVEL:
                current_scene = &level_scene;
                break;
            case BATTLE:
                current_scene = &battle_scene;
                break;
            case GAME_OVER:
                current_scene = &gameover_scene;
                break;
            case CREDITS:
                current_scene = &credits_scene;
                break;
        }

        if (current_scene == 0) {
            continue;
        }

        current_scene->update(current_scene, &hud);
        hud_update(current_scene, &hud);

        if (IsMusicValid(*current_scene->music)) {
            UpdateMusicStream(*current_scene->music);
        }

        // Draw
        // Screen
        BeginTextureMode(screenRenderer);
        {
            ClearBackground(RAYWHITE);

            current_scene->draw(current_scene->data);
            hud_draw(&hud);
        }
        EndTextureMode();

        BeginDrawing();

        const float virtualWidthRatio = (float)GetScreenWidth() / (float)SCREEN_WIDTH;
        const float virtualHeightRatio = (float)GetScreenHeight() / (float)SCREEN_HEIGHT;

        DrawTexturePro(
            screenRenderer.texture,
            (Rectangle){0, 0, (float)screenRenderer.texture.width, -(float)screenRenderer.texture.height},
            (Rectangle){
                -virtualWidthRatio, -virtualHeightRatio, (float) GetScreenWidth() + virtualWidthRatio * 2.0f,
                (float) GetScreenHeight() + virtualHeightRatio * 2.0f
            },
            (Vector2){0, 0},
            0.0f,
            WHITE
        );


        // DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, GREEN);

        EndDrawing();
        frame++;

        // Check if we need to change scenes
        if (current_scene->request_scene_change.type != NONE) {
            scene = current_scene->request_scene_change.type;

            struct Scene *new_scene = &title_scene;
            switch (scene) {
                case LEVEL:
                    new_scene = &level_scene;
                    break;
                case BATTLE:
                    new_scene = &battle_scene;
                    break;
                case GAME_OVER:
                    new_scene = &gameover_scene;
                    break;
                case CREDITS:
                    new_scene = &credits_scene;
                    break;
                case NONE:
                case TITLE:
                    break;
            }

            current_scene->unload(current_scene, &hud);
            if (current_scene->request_scene_change.callback != NULL) {
                current_scene->request_scene_change.callback(current_scene->data, new_scene, &hud);
            }
            new_scene->load(new_scene, &hud);

            if (current_scene->post_unload != NULL) {
                current_scene->post_unload(current_scene, &hud);
            }

            current_scene->request_scene_change.type = NONE;
            current_scene->request_scene_change.callback = NULL;
            frame = 0;
        }
    }

    UnloadRenderTexture(screenRenderer);

    title_scene.unload(&title_scene, &hud);

    level_scene.unload(&level_scene, &hud);

    battle_scene.unload(&battle_scene, &hud);

    credits_scene.unload(&credits_scene, &hud);

    array_free(&hud.dialogue);
    array_free(&hud.buttons);
    array_free(&player.inventory);

    assets_unload();

    CloseWindow();

    return 0;
}

void reset_game() {
    player.health = player.maxHealth;
    player.inventory.size = 0;
}

#include "enemy/type/hawk.h"

void debug(struct BattleData *data) {
    player_add_item(&player, (struct Item) {
        .type = ATTACK,
        .texture = &assets.texture_item_sword,
        .value = 10
    });

    player_add_item(&player, (struct Item) {
        .type = DEFENCE,
        .texture = &assets.texture_item_cloak,
        .value = 18
    });

    player_add_item(&player, (struct Item) {
        .type = ATTACK,
        .texture = &assets.texture_item_strength_potion,
        .value = 17
    });

    data->enemy = (Enemy) {
        .max_health = 3000,
        .health = 3000,
        .attack_stat = 18,
        .defence_stat = 15,
        .total_attacks = 4,

        .initialize = enemy_hawk_initialize,
        .unload = enemy_hawk_unload,
        .attack = enemy_hawk_attack,
        .pre_defeat = enemy_hawk_pre_defeat,
        .post_defeat = enemy_hawk_post_defeat
    };
}