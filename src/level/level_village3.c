#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"
#include "../player/player.h"
#include "../battle/battle.h"
#include "../battle/enemy/flowey.h"
#include "../screen/battle_screen.h"
#include "../asset_manager.h"

void loadLevelVillage3();
void unloadLevelVillage3();

void village3_battleFlowey();
void village3_goBack();

Level levelVillage3 = {
    .load = loadLevelVillage3,
    .unload = unloadLevelVillage3
};

Button* village3_buttons;

void loadLevelVillage3() {
    levelVillage3.texture = &assets.texture_level_village3;


    if (player.flags.boss_flowey) {
        village3_buttons = (Button*) malloc(sizeof(Button) * 1);

        setDialogue("Farmer Johan: Hey, how's it going?");

        village3_buttons[0] = (Button) {
            .text = "Go back",
            .onClick = village3_goBack
        };

        setButtons(village3_buttons, 1);
    } else {
        setDialogueMulti((char*[]) {"Farmer Johan: Could you help me defeat this monster?", "I'll give you a golden coin as a reward."}, 2);

        village3_buttons = (Button*) malloc(sizeof(Button) * 2);

        village3_buttons[0] = (Button) {
            .text = "Attack the enemy",
            .onClick = village3_battleFlowey
        };

        village3_buttons[1] = (Button) {
            .text = "Go back",
            .onClick = village3_goBack
        };

        setButtons(village3_buttons, 2);
    }

}

void unloadLevelVillage3() {
    free(village3_buttons);
}

void village3_battleFlowey() {
    Battle battle = {
        .enemy = (Enemy) {
            .max_health = 30,
            .health = 30,
            .attack_stat = 2,
            .defence_stat = 2,
            .total_attacks = 1,

            .initialize = enemy_flowey_initialize,
            .unload = enemy_flowey_unload,
            .attack = enemy_flowey_attack,
            .pre_defeat = enemy_flowey_pre_defeat,
            .post_defeat = enemy_flowey_post_defeat
        },
    };

    start_battle(battle);
}

void village3_goBack() {
    changeLevel(&levelVillage1);
}
