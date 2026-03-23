#include <raylib.h>
#include <stdlib.h>

#include "level/level.h"
#include "hud.h"
#include "player/player.h"
#include "screen/game_screen.h"
#include "battle/battle.h"
#include "enemy/bear.h"
#include "screen/battle_screen.h"
#include "asset_manager.h"

void loadLevelForest2();
void unloadLevelForest2();

void forest2_attackBear();
void forest2_goForward();
void forest2_goBack();

Level levelForest2 = {
    .load = loadLevelForest2,
    .unload = unloadLevelForest2
};

Button* forest2_buttons;

void loadLevelForest2() {
    if (player.flags.boss_bear)
        levelForest2.texture = &assets.texture_level_forest2_nobear;
    else
        levelForest2.texture = &assets.texture_level_forest2_bear;

    forest2_buttons = (Button*) malloc(sizeof(Button) * 2);

    if (player.flags.boss_bear) { // TODO boss bear defeated
        setDialogueMulti((char*[]) {"Voiceover Pete: Alright, the bear is gone.", "You may now proceed safely."}, 2);
        forest2_buttons[0] = (Button) {
            .text = "Go forward",
            .onClick = forest2_goForward
        };
    } else {
        setDialogueMulti((char*[]) {
            "Voiceover Pete: Attention all humans, this bear is really dangerous.",
            "You gotta help us out!",
            "The only thing you need to do is to fight the bear",
            "with a sword!",
            "",
            false ? "You have a sword with you, what are you waiting for?!" : "You dont have a sword? You can get one at the village."}, 2); // TODO has sword

        forest2_buttons[0] = (Button) {
            .text = "Attack the bear",
            .onClick = forest2_attackBear
        };
    }

    forest2_buttons[1] = (Button) {
        .text = "Go back",
        .onClick = forest2_goBack
    };

    setButtons(forest2_buttons, 2);
}

void unloadLevelForest2() {
    free(forest2_buttons);
}

void forest2_attackBear() {
    Battle battle = {
        .enemy = (Enemy) {
            .max_health = 250,
            .health = 250,
            .attack_stat = 2,
            .defence_stat = 8,
            .total_attacks = 3,

            .initialize = enemy_bear_initialize,
            .unload = enemy_bear_unload,
            .attack = enemy_bear_attack,
            .pre_defeat = enemy_bear_pre_defeat,
            .post_defeat = enemy_bear_post_defeat
        },
    };

    start_battle(battle);
}

void forest2_goForward() {
    changeLevel(&levelForest3);
}

void forest2_goBack() {
    changeLevel(&levelForest1);
}