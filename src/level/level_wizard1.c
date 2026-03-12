#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"

void loadLevelWizard1();
void unloadLevelWizard1();

void wizard1_battleGolem();
void wizard1_enterHouse();
void wizard1_goBack();

Level levelWizard1 = {
    .load = loadLevelWizard1,
    .unload = unloadLevelWizard1
};

Button* wizzard1_buttons;

void loadLevelWizard1() {
    levelWizard1.texture = LoadTexture("assets/textures/levels/wizard1.png");

    wizzard1_buttons = (Button*) malloc(sizeof(Button) * 2);
    
    if (true) { // TODO boss golem defeated
        if (false) { // TODO boss wizard defeated
            setDialogue("It feels empty here...");
        } else {
            setDialogue("Wizard: So, I see you defeated my golem, well done.");
        }

        wizzard1_buttons[0] = (Button){
            .text = "Enter house",
            .onClick = wizard1_enterHouse
        };
    } else {
        setDialogueMulti((char*[]) {"???: I see, your adventure is almost over. But first,", "I'm gonna heal you, because...", "you must defeat my mighty GOLEM to continue on!"}, 3);

        wizzard1_buttons[0] = (Button){
            .text = "Attack golem",
            .onClick = wizard1_battleGolem
        };
    }

    wizzard1_buttons[1] = (Button) {
        .text = "Go back",
        .onClick = wizard1_goBack
    };

    setButtons(wizzard1_buttons, 2);
}

void unloadLevelWizard1() {
    free(wizzard1_buttons);
    UnloadTexture(levelWizard1.texture);
}

void wizard1_battleGolem() {

}

void wizard1_enterHouse() {
    changeLevel(&levelWizard2);
}

void wizard1_goBack() {
    changeLevel(&levelForest3);
}