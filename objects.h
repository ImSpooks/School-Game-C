#ifndef TESTRAYLIB_OBJECTS_H
#define TESTRAYLIB_OBJECTS_H

typedef enum ItemType {
    ATTACK,
    DEFENCE,
    KEY_ITEM,
    SELLABLE
} ItemType;

typedef struct Item {
    ItemType type;
    char* name;
    int value;
} Item;

typedef struct Player {
    Item* inventory;
    int health;
    int maxHealth;
} Player;

extern Player player;





#endif //TESTRAYLIB_OBJECTS_H