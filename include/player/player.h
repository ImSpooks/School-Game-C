#ifndef TESTRAYLIB_PLAYER_H
#define TESTRAYLIB_PLAYER_H

#include "items.h"

#define PLAYER_SPEED 128

typedef struct PlayerFlags {
    bool boss_flowey;
    bool boss_bear;
    bool boss_golem;
    bool boss_wizard;
    bool boss_hawk;
    bool boss_nick;
    bool boss_renoir;

    bool paid_pirate;
    bool healed_angel;
} PlayerFlags;

typedef struct Player {
    Item* inventory;
    int health;
    int maxHealth;
    PlayerFlags flags;

    Vector2 position;
    float invincibility_timer;
} Player;

extern Player player;

float get_attack_stat();
float get_defence_stat();

void remove_item_index(int index);
void remove_item_type(ItemType type);
bool contains_item(ItemType type);

#endif //TESTRAYLIB_PLAYER_H