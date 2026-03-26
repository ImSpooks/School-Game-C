#ifndef TESTRAYLIB_PLAYER_H
#define TESTRAYLIB_PLAYER_H

#include "util/array.h"
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

struct Player {
    struct Array inventory;
    PlayerFlags flags;

    float health;
    float maxHealth;

    Vector2 position;
    float invincibility_timer;
};

float player_get_stat(struct Player player, enum ItemType type);
float player_get_attack(struct Player player);
float player_get_defence(struct Player player);

void player_add_item(struct Player *player, struct Item item);
void player_remove_item(struct Player *player, enum ItemType type);
bool player_contains_item(struct Player player, enum ItemType type);

extern struct Player player;

#endif //TESTRAYLIB_PLAYER_H