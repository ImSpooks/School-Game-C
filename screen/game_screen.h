#ifndef TESTRAYLIB_GAME_SCREEN_H
#define TESTRAYLIB_GAME_SCREEN_H

#include "screen.h"
#include "../level/level.h"

#define TRANSITION_LENGTH CLITERAL((float) 1)

void setLevel(Level* level);
Level* getLevel();
void changeLevel(Level* level);

#endif //TESTRAYLIB_GAME_SCREEN_H