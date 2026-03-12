#include <raylib.h>
#include <stdlib.h>

#include "level.h"
#include "../hud.h"
#include "../screen/game_screen.h"

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
    levelForest2.texture = LoadTexture("assets/textures/levels/forest2.png");

    forest2_buttons = (Button*) malloc(sizeof(Button) * 2);

    if (true) { // TODO boss bear defeated
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
    UnloadTexture(levelForest2.texture);
}

void forest2_attackBear() {

}

void forest2_goForward() {
    changeLevel(&levelForest3);
}

void forest2_goBack() {
    changeLevel(&levelForest1);
}