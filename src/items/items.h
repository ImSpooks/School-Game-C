#ifndef TESTRAYLIB_OBJECTS_H
#define TESTRAYLIB_OBJECTS_H
#include <raylib.h>

typedef enum ItemType {
    ATTACK,
    DEFENCE,
    SELLABLE,
    COIN,
    POTION
} ItemType;

typedef struct Item {
    ItemType type;
    Texture2D* texture;
    float value;
} Item;

#endif //TESTRAYLIB_OBJECTS_H