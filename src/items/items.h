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

extern Texture2D item_texture_bear_tooth;
extern Texture2D item_texture_cloak;
extern Texture2D item_texture_coin;
extern Texture2D item_texture_golem_rock;
extern Texture2D item_texture_heal_potion;
extern Texture2D item_texture_strength_potion;
extern Texture2D item_texture_sword;

void load_items();
void unload_items();

#endif //TESTRAYLIB_OBJECTS_H