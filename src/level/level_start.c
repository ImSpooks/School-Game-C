#include <raylib.h>
#include <stdlib.h>

#include "level/level.h"
#include "hud.h"
#include "player/player.h"
#include "screen/game_screen.h"
#include "screen/screen.h"
#include "battle/battle.h"
#include "enemy/hawk.h"
#include "screen/battle_screen.h"
#include "asset_manager.h"

void loadLevelStart();
void unloadLevelStart();

void start_goIntoTheForest();
void start_giveHealthPotion();
void start_talkToAngel();
void start_attackHawk();

Level levelStart = {
    .load = loadLevelStart,
    .unload = unloadLevelStart,
};

Button* start_buttons;

bool firstLoaded = false;

void loadLevelStart() {
    levelStart.texture = &assets.texture_level_start;

    if (player.flags.boss_hawk) {
        // TODO: Game ending
        setDialogue("TODO: Game ending");
    } else {
        if (firstLoaded) {
            firstLoaded = true;
            setDialogue("You woke up, in this strange forest.");
        } else {
            setDialogue("Your adventure started here.");
        }


        start_buttons = (Button*) malloc(sizeof(Button) * 2);

        start_buttons[0] = (Button) {
            .text = "Go into the woods",
            .onClick = start_goIntoTheForest,
        };

        if (contains_item(POTION)) {
            start_buttons[1] = (Button) {
                .text = "Give health potion",
                .onClick = start_giveHealthPotion
            };
        } else {
            start_buttons[1] = (Button) {
                .text = "Talk to the Angel",
                .onClick = start_talkToAngel,
            };
        }


        setButtons(start_buttons, 2);
    }
}

void unloadLevelStart() {
    free(start_buttons);
}

void start_goIntoTheForest() {
    changeLevel(&levelForest1);
}

void start_giveHealthPotion() {
    setDialogue("Angel: Thank you! Now help me defeat this monster!");

    start_buttons = (Button*) malloc(sizeof(Button) * 1);

    start_buttons[0] = (Button) {
        .text = "Attack the giant Hawk",
        .onClick = start_attackHawk,
    };

    setButtons(start_buttons, 1);

    player.flags.healed_angel = true;
}

void start_talkToAngel() {
    setDialogue("Angel: I got weakened by this giant hawk, please find a healing potion.");
}

void start_attackHawk() {
    Battle battle = {
        .enemy = (Enemy) {
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
        },
    };
    player.health = player.maxHealth;

    start_battle(battle);
}