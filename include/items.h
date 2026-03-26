#ifndef TESTRAYLIB_OBJECTS_H
#define TESTRAYLIB_OBJECTS_H
#include <raylib.h>

enum ItemType {
    ATTACK,
    DEFENCE,
    SELLABLE,
    COIN,
    POTION
};

struct Item {
    enum ItemType type;
    Texture2D* texture;
    float value;
};

#endif //TESTRAYLIB_OBJECTS_H