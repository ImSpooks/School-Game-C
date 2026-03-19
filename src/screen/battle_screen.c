#include "battle_screen.h"

#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>

#include "game_screen.h"
#include "../globals.h"
#include "screen.h"
#include "vec.h"
#include "../battle/battle.h"
#include "../hud.h"
#include "../player/player.h"
#include "../util/util.h"

void initializeBattleScreen();
void unloadBattleScreen();
void drawBattleScreen(const RenderTexture2D* texture);
void updateBattleScreen(const RenderTexture2D* texture);

void battle_attack();

void end_battle_button();

Screen battleScreen = {
    .initialize = initializeBattleScreen,
    .unload = unloadBattleScreen,
    .update = updateBattleScreen,
    .draw = drawBattleScreen,
};

Battle activeBattle;
BattleOptions battleOptions;
static bool pendingEndBattle = false;

Texture2D texture_damage_bar;
Texture2D texture_damage_slider;
Texture2D texture_damage_slice;

Texture2D texture_soul;

Sound sound_attack;
Sound sound_damage_enemy;
Sound sound_damage_take;
Sound sound_victory;


Button* battle_buttons;

void initializeBattleScreen() {
    pendingEndBattle = false;
    setDialogue("This is the battle screen");

    texture_damage_bar = LoadTexture("assets/textures/ui/damage_bar.png");
    texture_damage_slider = LoadTexture("assets/textures/ui/damage_slider.png");
    texture_damage_slice = LoadTexture("assets/textures/ui/damage_slice.png");
    texture_soul = LoadTexture("assets/textures/ui/soul.png");

    sound_attack = LoadSound("assets/sfx/attack.wav");
    sound_damage_enemy = LoadSound("assets/sfx/damage_enemy.wav");
    sound_damage_take = LoadSound("assets/sfx/damage_take.wav");
    sound_victory = LoadSound("assets/sfx/victory.wav");

    activeBattle.enemy.initialize(&activeBattle.enemy);
    activeBattle.enemy.projectiles = vector_create();

    battle_buttons = (Button*)malloc(sizeof(Button) * 1);

    battle_buttons[0] = (Button){
        .text = "Attack",
        .onClick = battle_attack
    };

    setButtons(battle_buttons, 1);

    PlayMusicStream(activeBattle.enemy.music);

    hud.render_items = false;
    hud.render_health = true;
}

void unloadBattleScreen() {
    if (IsMusicStreamPlaying(activeBattle.enemy.music))
        StopMusicStream(activeBattle.enemy.music);
    if (IsMusicValid(activeBattle.enemy.music2) && IsMusicStreamPlaying(activeBattle.enemy.music2))
        StopMusicStream(activeBattle.enemy.music2);

    printf("UNLOADING BATTLESCREEN");

    UnloadTexture(texture_damage_bar);
    UnloadTexture(texture_damage_slider);
    UnloadTexture(texture_damage_slice);
    UnloadTexture(texture_soul);

    UnloadSound(sound_attack);
    UnloadSound(sound_damage_enemy);
    UnloadSound(sound_damage_take);

    UnloadSound(sound_victory);

    activeBattle.enemy.unload(&activeBattle.enemy);
    if (battleOptions.state == ENEMY) {
        // Should only happen on force close
        for (int i = 0; i < vector_size(activeBattle.enemy.projectiles); i++) {
            Projectile* projectile = activeBattle.enemy.projectiles + i;
            free(projectile->data);
        }
        vector_free(activeBattle.enemy.projectiles);
    }

    free(battle_buttons);

    hud.render_items = true;
    hud.render_health = false;
}

void updateBattleScreen(const RenderTexture2D* texture) {
    if (pendingEndBattle) {
        pendingEndBattle = false;
        activeBattle.enemy.pre_defeat();
        setScreen(&gameScreen);
        activeBattle.enemy.post_defeat();
        return;
    }

    if (!IsMusicValid(activeBattle.enemy.music2)) {
        UpdateMusicStream(activeBattle.enemy.music);
    } else {
        if (IsMusicStreamPlaying(activeBattle.enemy.music2)) {
            UpdateMusicStream(activeBattle.enemy.music2);
        } else {
            if (!IsMusicStreamPlaying(activeBattle.enemy.music)) {
                if (!IsMusicStreamPlaying(activeBattle.enemy.music2)) {
                    StopMusicStream(activeBattle.enemy.music);
                    PlayMusicStream(activeBattle.enemy.music2);
                }
            } else {
                UpdateMusicStream(activeBattle.enemy.music);
            }
        }
    }

    float frame_time = GetFrameTime();

    if (battleOptions.state == CALCULATING_DAMAGE) {
        if (battleOptions.timer >= CALCULATING_DAMAGE_LENGTH) {
            battleOptions.timer = 0;
            battleOptions.state = DEALING_DAMAGE;

            PlaySound(sound_attack);
        }
    } else if (battleOptions.state == DEALING_DAMAGE) {
        if (battleOptions.timer >= DEALING_DAMAGE_LENGTH) {
            battleOptions.timer = 0;
            battleOptions.state = MOVE_HEALTHBAR;

            setButtons(NULL, 0);
            PlaySound(sound_damage_enemy);
        }
    } else if (battleOptions.state == MOVE_HEALTHBAR) {
        if (battleOptions.timer >= RENDERING_DAMAGE_LENGTH) {
            battleOptions.timer = 0;
            battleOptions.state = ENEMY;

            activeBattle.enemy.health -= battleOptions.last_damage;
            battleOptions.enemy_attack = floor((double)rand() / (double) RAND_MAX * activeBattle.enemy.total_attacks);

            player.position.x = BATTLE_BOUNDS_MIDDLE.x;
            player.position.y = BATTLE_BOUNDS_MIDDLE.y;
            player.invincibility_timer = 0;

            if (activeBattle.enemy.health > 0)
                activeBattle.enemy.projectiles = vector_create();
        }
    } else if (battleOptions.state == ENEMY) {
        if (activeBattle.enemy.health <= 0) {
            battleOptions.state = FINISHED;
            end_battle();

        } else if (battleOptions.timer > BATTLE_BOUNDS_EXPAND_TIME) {
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

            if (activeBattle.enemy.attack(&activeBattle.enemy.projectiles, battleOptions.enemy_attack, battleOptions.timer - BATTLE_BOUNDS_EXPAND_TIME, battleOptions.turn)) {
                battleOptions.timer = 0;
                battleOptions.state = ENEMY_FINISH;
                battleOptions.turn++;

                setButtons(battle_buttons, 1);

                for (int i = 0; i < vector_size(activeBattle.enemy.projectiles); i++) {
                    Projectile* projectile = activeBattle.enemy.projectiles + i;
                    free(projectile->data);
                }
                vector_free(activeBattle.enemy.projectiles);
            } else {
                Rectangle playerHitbox = (Rectangle) {player.position.x - 8, player.position.y - 8, 16, 16};

                for (int i = 0; i < (int)vector_size(activeBattle.enemy.projectiles); i++) {
                    Projectile* projectile = activeBattle.enemy.projectiles + i;

                    Rectangle hitbox = projectile->get_hitbox(projectile);

                    // Remove projectiles outside of bounds
                    if (!CheckCollisionRecs(hitbox, (Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}) || projectile->lifespan > 99) {
                        free(projectile->data);
                        vector_remove(activeBattle.enemy.projectiles, i--);
                        continue;
                    }

                    if (player.invincibility_timer <= 0) {
                        if (CheckCollisionRecs(playerHitbox, hitbox)) {
                            damage_player(projectile, projectile->true_damage);
                            PlaySound(sound_damage_take);
                        }
                    }

                    projectile->lifespan += frame_time;
                }
            }
        }
    } else if (battleOptions.state == ENEMY_FINISH) {
        if (battleOptions.timer >= BATTLE_BOUNDS_EXPAND_TIME) {
            battleOptions.timer = 0;
            battleOptions.state = PLAYER_TURN;
        }
    }

    battleOptions.timer += frame_time;


}

void drawBattleScreen(const RenderTexture2D* texture) {
    ClearBackground(BLACK);

    for (int x = 0; x < 640; x += 64) {
        DrawLine(x, 0, x, 360, GREEN);
    }

    for (int y = 0; y < 360; y += 60) {
        DrawLine(0, y, 640, y, GREEN);
    }

    int y = 64 - (activeBattle.enemy.texture.height / 2);
    if (y < 16) y = 16;

    if (activeBattle.enemy.health > 0) {
        Vector2 pos = {640 / 2.0f - activeBattle.enemy.texture.width / 2.0f, y};
        DrawTexture(activeBattle.enemy.texture, (int)pos.x, (int)pos.y, WHITE);
    }

    if (battleOptions.state == CALCULATING_DAMAGE) {
        DrawTexture(texture_damage_bar, 640 / 2 - texture_damage_bar.width / 2, 360 - 162, WHITE);

        int startPos = 640 / 2 - texture_damage_bar.width / 2;
        int endPos   = 640 / 2 + texture_damage_bar.width / 2;
        float percent = battleOptions.timer / CALCULATING_DAMAGE_LENGTH;
        int pos = startPos + (endPos - startPos) * percent;

        DrawTexture(texture_damage_slider, pos - texture_damage_slider.width / 2, 360 - 162 + texture_damage_bar.height / 2 - texture_damage_slider.height / 2, WHITE);


    } else if (battleOptions.state == DEALING_DAMAGE) {
        int sprite = (int)floor((battleOptions.timer / DEALING_DAMAGE_LENGTH) * 6);
        int width = texture_damage_slice.width / 6;

        DrawTextureRec(texture_damage_slice, (Rectangle) {sprite * width, 0, width, texture_damage_slice.height},
            (Vector2){640 / 2 - width / 2, (y + activeBattle.enemy.texture.height / 2) - (texture_damage_slice.height/2)}, WHITE);
    } else if (battleOptions.state == MOVE_HEALTHBAR){
        const char* text = TextFormat("%d", (int) ceilf(battleOptions.last_damage));
        int font_size = 25;
        int width = MeasureText(text, font_size);

        Vector2 pos = {(SCREEN_WIDTH / 2) - (width / 2), y + activeBattle.enemy.texture.height / 4 * 3};

        DrawOutlinedText(text,
            (int) (pos.x + ((double) rand() / (double) RAND_MAX - 0.5) * 4),
            (int) ((pos.y - font_size / 2) + ((double) rand() / (double) RAND_MAX - 0.5) * 4),
            font_size, RED, 3, BLACK
        );

        const float maxHealth = activeBattle.enemy.max_health;
        const float currentHealth = activeBattle.enemy.health;

        const float renderHealth = currentHealth - (battleOptions.last_damage * (battleOptions.timer / RENDERING_DAMAGE_LENGTH));

        const float healthBarWidth = 100;
        float renderWitdh = healthBarWidth * (renderHealth / maxHealth);
        if (renderWitdh < 0)
            renderWitdh = 0;

        DrawRectangle((SCREEN_WIDTH / 2) - healthBarWidth / 2, pos.y + font_size + 2, (int)healthBarWidth, 12, DARKGRAY);
        DrawRectangle((SCREEN_WIDTH / 2) - healthBarWidth / 2, pos.y + font_size + 2, (int)renderWitdh, 12, RED);
        DrawRectangleLinesEx((Rectangle) {(SCREEN_WIDTH / 2) - healthBarWidth / 2, pos.y + font_size + 2, healthBarWidth, 12}, 1, BLACK);

    } else if (battleOptions.state == ENEMY) {
        const int line_width = 3;
        if (battleOptions.timer < BATTLE_BOUNDS_EXPAND_TIME) {
            const float percent = battleOptions.timer / BATTLE_BOUNDS_EXPAND_TIME;

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
            int width = texture_soul.width / 2;
            int sprite = 0;
            if (player.invincibility_timer > 0) {
                sprite = 1;
            }

            DrawTextureRec(texture_soul, (Rectangle) {sprite * width, 0, width, texture_soul.height},
                (Vector2){player.position.x - width / 2, player.position.y - texture_soul.height / 2}, WHITE);


            size_t size = vector_size(activeBattle.enemy.projectiles);
            for (int i = 0; i < size; i++) {
                Projectile *projectile = activeBattle.enemy.projectiles + i;
                projectile->draw(projectile);
            }
        }
    } else if (battleOptions.state == ENEMY_FINISH) {
        const int line_width = 3;
        const float percent = 1 - (battleOptions.timer / BATTLE_BOUNDS_EXPAND_TIME);

        const float expansionX = BATTLE_BOUNDS.width / 2 * percent;
        const float expansionY = BATTLE_BOUNDS.height / 2 * percent;

        Rectangle expandedRect = {
            BATTLE_BOUNDS_MIDDLE.x - expansionX, BATTLE_BOUNDS_MIDDLE.y - expansionY, expansionX * 2, expansionY * 2
        };

        DrawRectangleRec(expandedRect, BLACK);
        DrawRectangleLinesEx(expandedRect, line_width, WHITE);
    }
}

void start_battle(Battle battle) {
    activeBattle = battle;
    battleOptions = (BattleOptions){
        .state = PLAYER_TURN,
        .timer = 0,
        .enemy_attack = 0,
        .turn = 0
    };

    setScreen(&battleScreen);
}

void end_battle() {
    battle_buttons[0].text = "Continue";
    battle_buttons[0].onClick = end_battle_button;
    setButtons(battle_buttons, 1);

    SetMusicVolume(activeBattle.enemy.music, 0.4f);
    if (IsMusicValid(activeBattle.enemy.music2))
        SetMusicVolume(activeBattle.enemy.music2, 0.4f);
    PlaySound(sound_victory);
}

void battle_attack() {
    if (battleOptions.state == PLAYER_TURN) {
        setDialogue("");
        battleOptions.state = CALCULATING_DAMAGE;
        battleOptions.timer = 0;
        battleOptions.last_damage = 0;
    } else if (battleOptions.state == CALCULATING_DAMAGE) {

        float percent = battleOptions.timer / CALCULATING_DAMAGE_LENGTH; // 0 to 1 range, 0.5 is optimal damage
        float damage = sinf(percent * PI) * 15;

        damage = damage * get_attack_stat() / sqrtf(powf((float)activeBattle.enemy.defence_stat, 1.75f));
        damage = damage * (powf((float)battleOptions.turn / 10.0f, 2) + 1);

        battleOptions.last_damage = damage * 1;

        battleOptions.timer = 0;
        battleOptions.state = DEALING_DAMAGE;

        setButtons(NULL, 0);
        PlaySound(sound_attack);
    }
}

void end_battle_button() {
    setButtons(NULL, 0);
    // do stuff next frame
    pendingEndBattle = true;
}