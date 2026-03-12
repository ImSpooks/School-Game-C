#include "battle_screen.h"

#include <math.h>
#include <raylib.h>
#include <stddef.h>

#include "screen.h"
#include "vec.h"
#include "../battle/battle.h"
#include "../hud.h"
#include "../player/player.h"

void initializeBattleScreen();
void unloadBattleScreen();
void drawBattleScreen(const RenderTexture2D* texture);
void updateBattleScreen(const RenderTexture2D* texture);

void battle_attack();

Screen battleScreen = {
    .initialize = initializeBattleScreen,
    .unload = unloadBattleScreen,
    .update = updateBattleScreen,
    .draw = drawBattleScreen,
};

Battle activeBattle;
BattleOptions battleOptions;

Texture2D texture_damage_bar;
Texture2D texture_damage_slider;
Texture2D texture_damage_slice;

Texture2D texture_soul;

Button* battle_buttons;

void initializeBattleScreen() {
    setDialogue("This is the battle screen");

    texture_damage_bar = LoadTexture("assets/textures/ui/damage_bar.png");
    texture_damage_slider = LoadTexture("assets/textures/ui/damage_slider.png");
    texture_damage_slice = LoadTexture("assets/textures/ui/damage_slice.png");
    texture_soul = LoadTexture("assets/textures/ui/soul.png");

    activeBattle.enemy.initialize(&activeBattle.enemy);
    activeBattle.enemy.projectiles = vector_create();

    battle_buttons = (Button*)malloc(sizeof(Button) * 1);

    battle_buttons[0] = (Button){
        .text = "Attack",
        .onClick = battle_attack
    };

    setButtons(battle_buttons, 1);

}

void unloadBattleScreen() {
    UnloadTexture(texture_damage_bar);
    UnloadTexture(texture_damage_slider);
    UnloadTexture(texture_damage_slice);
    UnloadTexture(texture_soul);

    vector_free(activeBattle.enemy.projectiles);
    activeBattle.enemy.unload(&activeBattle.enemy);
}

void updateBattleScreen(const RenderTexture2D* texture) {
    float frame_time = GetFrameTime();

    if (battleOptions.state == CALCULATING_DAMAGE) {
        if (battleOptions.timer >= 1) {
            battleOptions.timer = 0;
            battleOptions.state = DEALING_DAMAGE;

            // TODO: sfx attack.wav
        }
    } else if (battleOptions.state == DEALING_DAMAGE) {
        if (battleOptions.timer >= 1) {
            battleOptions.timer = 0;
            battleOptions.state = MOVE_HEALTHBAR;

            setButtons(NULL, 0);
            // TODO: sfx damage_enemy.wav
        }
    } else if (battleOptions.state == MOVE_HEALTHBAR) {
        if (battleOptions.timer >= 1) {
            battleOptions.timer = 0;
            battleOptions.state = ENEMY;

            battleOptions.enemy_attack = floor((double)rand() / (double) RAND_MAX * activeBattle.enemy.total_attacks);
        }
    } else if (battleOptions.state == ENEMY) {
        if (activeBattle.enemy.attack(activeBattle.enemy.projectiles, battleOptions.enemy_attack, battleOptions.timer, battleOptions.turn)) {
            battleOptions.timer = 0;
            battleOptions.state = PLAYER_TURN;
            battleOptions.turn++;

            setButtons(battle_buttons, 1);
        }
    }

    size_t size = vector_size(activeBattle.enemy.projectiles);
    for (int i = 0; i < size; i++) {
        Projectile* projectile = activeBattle.enemy.projectiles + i;
        projectile->lifespan += frame_time;
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
    if (y < 32) y = 32;

    if (battleOptions.state == CALCULATING_DAMAGE) {
        DrawTexture(texture_damage_bar, 640 / 2 - texture_damage_bar.width / 2, 360 - 130, WHITE);

        int startPos = 640 / 2 - texture_damage_bar.width / 2;
        int endPos   = 640 / 2 + texture_damage_bar.width / 2;
        float percent = battleOptions.timer;
        int pos = startPos + (endPos - startPos) * percent;

        DrawTexture(texture_damage_slider, pos - texture_damage_bar.width / 2, 360 - 130 + texture_damage_bar.height / 2 - texture_damage_bar.height / 2, WHITE);


    } else if (battleOptions.state == DEALING_DAMAGE) {
        int sprite = (int)floor(battleOptions.timer * 6);
        int width = texture_damage_slice.width / 6;

        DrawTextureRec(texture_damage_slice, (Rectangle) {sprite * width, 0, width, texture_damage_slice.height}, (Vector2){640 / 2 - width / 2, y}, WHITE);
    } else if (battleOptions.state == MOVE_HEALTHBAR){
        const char* text = TextFormat("%d", (int) ceilf(battleOptions.last_damage));
        int width = MeasureText(text, 10);

        Vector2 pos = {(640 / 2) - (width / 2) + activeBattle.enemy.texture.height / 4 * 3};

        DrawText(text,
            (int) (pos.x + ((double) rand() / (double) RAND_MAX - 0.5) * 4),
            (int) (pos.y + ((double) rand() / (double) RAND_MAX - 0.5) * 4),
            10, RED
        );

        int maxHealth = activeBattle.enemy.max_health;
        int currentHealth = activeBattle.enemy.health;
        int newHealth = currentHealth - battleOptions.last_damage;
        int difference = currentHealth - newHealth;

        int healthBarWidth = 100;
        //int renderWitdh = healthBarWidth * ()

    }

    if (activeBattle.enemy.health > 0) {
        Vector2 pos = {640 / 2.0f - activeBattle.enemy.texture.width / 2.0f, y < 32 ? 32 : y};
        DrawTexture(activeBattle.enemy.texture, (int)pos.x, (int)pos.y, WHITE);
    }

    size_t size = vector_size(activeBattle.enemy.projectiles);
    for (int i = 0; i < size; i++) {
        activeBattle.enemy.projectiles[i].draw();
    }

    if (battleOptions.state == CALCULATING_DAMAGE) {
        DrawTexture(texture_damage_bar, 640 / 2 - texture_soul.width / 2, 360 - 130, WHITE);

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
    // TODO: ok button
    //setScreen(&gameScreen);

    activeBattle.enemy.defeat();
}

void battle_attack() {
    if (battleOptions.state == PLAYER_TURN) {
        setDialogue("");
        battleOptions.state = CALCULATING_DAMAGE;
        battleOptions.timer = 0;
        battleOptions.last_damage = 0;
    } else if (battleOptions.state == CALCULATING_DAMAGE) {
        battleOptions.state = DEALING_DAMAGE;

        float percent = battleOptions.timer; // 0 to 1 range, 0.5 is optimal damage
        float damage = sinf(percent * PI) * 15;

        damage = damage * get_attack_stat() / powf((float)activeBattle.enemy.defence_stat, 1.75f);
        damage = damage * (powf((float)battleOptions.turn / 10.0f, 2) + 1);

        battleOptions.last_damage = damage;

        setButtons(NULL, 0);
        // TODO: sfx attack.wav
    }
}