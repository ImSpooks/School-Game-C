#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"
#include "../player/player.h"
#include "../battle/battle.h"
#include "../battle/enemy/wizard.h"
#include "../screen/battle_screen.h"
#include "../asset_manager.h"

void loadLevelWizard2();
void unloadLevelWizard2();

void wizard2_battleWizard();
void wizard2_goUpstairs();
void wizard2_goBack();

Level levelWizard2 = {
    .load = loadLevelWizard2,
    .unload = unloadLevelWizard2
};

Button* wizzard2_buttons;

void loadLevelWizard2() {
    levelWizard2.texture = &assets.texture_level_wizard2;

    wizzard2_buttons = (Button*) malloc(sizeof(Button) * 2);

    if (player.flags.boss_wizard) { // TODO boss wizard defeated
        setDialogue("This looks like a neat place");
        wizzard2_buttons[0] = (Button) {
            .text = "Go upstairs",
            .onClick = wizard2_goUpstairs
        };
    } else {
        setDialogueMulti((char*[]) {"Wizard: Before you search everything in this house...", "... you need to battle me first!"}, 2);
        wizzard2_buttons[0] = (Button) {
            .text = "Attack wizard",
            .onClick = wizard2_battleWizard
        };
    }

    wizzard2_buttons[1] = (Button) {
        .text = "Go back",
        .onClick = wizard2_goBack
    };

    setButtons(wizzard2_buttons, 2);
}

void unloadLevelWizard2() {
    free(wizzard2_buttons);
}

void wizard2_battleWizard() {
    Battle battle = {
        .enemy = (Enemy) {
            .max_health = 2500,
            .health = 2500,
            .attack_stat = 16,
            .defence_stat = 10,
            .total_attacks = 4,

            .initialize = enemy_wizard_initialize,
            .unload = enemy_wizard_unload,
            .attack = enemy_wizard_attack,
            .pre_defeat = enemy_wizard_pre_defeat,
            .post_defeat = enemy_wizard_post_defeat
        },
    };

    start_battle(battle);
}

void wizard2_goUpstairs() {
    changeLevel(&levelWizard3);
}

void wizard2_goBack() {
    changeLevel(&levelWizard1);
}