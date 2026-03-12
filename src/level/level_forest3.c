#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"

void loadLevelForest3();
void unloadLevelForest3();

void forest3_payPirate();
void forest3_toWizard();
void forest3_goBack();

Level levelForest3 = {
    .load = loadLevelForest3,
    .unload = unloadLevelForest3
};

Button* forest3_buttons;

void loadLevelForest3() {
    levelForest3.texture = LoadTexture("assets/textures/levels/forest3.png");

    forest3_buttons = (Button*) malloc(sizeof(Button) * 2);

    if (true) { // TODO we have paid the pirate
        forest3_buttons[0] = (Button) {
            .text = "Go to the wizard",
            .onClick = forest3_toWizard
        };
    } else {
        if (false) { // TODO has gold coin
            setDialogueMulti((char*[]) {
                "Pirate: Hello pal, are you trying to go on?",
                "You can only pass us by paying us atleast",
                "10 gold, so what are you waiting for?"
            }, 3);
        } else {
            setDialogueMulti((char*[]) {
                "Pirate: Hello pal, are you trying to go on?",
                "You can only pass us by paying us atleast",
                "10 gold, so what are you waiting for?",
                " ",
                "What? You dont have any gold? You might be able to",
                "sell some stuff at the merchant in the nearest village."
            }, 6);
        }
        forest3_buttons[0] = (Button) {
            .text = "Pay the pirate",
            .onClick = forest3_payPirate
        };
    }

    forest3_buttons[1] = (Button) {
        .text = "Go back",
        .onClick = forest3_goBack
    };

    setButtons(forest3_buttons, 2);
}

void unloadLevelForest3() {
    free(forest3_buttons);
    UnloadTexture(levelForest3.texture);
}

void forest3_payPirate() {
    // TODO has gold coin
    if (false) {
        // TODO pay pirate
        reloadLevel();
        setDialogueMulti((char*[]) {"Pirate: That is what I call a good deal, but take this.", "It will protect you against some monsters.", "And I'll give you this potion as well", "it'll boost your attack."}, 4);
    } else {
        setDialogue("You do not have a golden coin.");
    }
}

void forest3_toWizard() {
    changeLevel(&levelWizard1);
}

void forest3_goBack() {
    changeLevel(&levelForest2);
}