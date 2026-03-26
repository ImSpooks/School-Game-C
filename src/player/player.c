#include "player/player.h"


float player_get_stat(struct Player player, enum ItemType type){
    float base = 1.0f;

    const int size = player.inventory.size;
    for (int i = 0; i < size; i++) {
        struct Item item = ((struct Item*)player.inventory.data)[i];
        if (item.type == type) {
            base += item.value;
        }
    }
    return base;
}

float player_get_attack(struct Player player) {
    return player_get_stat(player, ATTACK);
}
float player_get_defence(struct Player player) {
    return player_get_stat(player, DEFENCE);
}

void player_add_item(struct Player *player, struct Item item) {

    const int size = player->inventory.size;
    array_allocate(&player->inventory, sizeof(struct Item), size + 1);


    ((struct Item*)player->inventory.data)[size] = item;

    player->inventory.size = size + 1;
}

void player_remove_item(struct Player *player, enum ItemType type) {

    int size = player->inventory.size;
    struct Item *items = (struct Item*)player->inventory.data;

    for (int i = 0; i < size; i++) {
        if (items[i].type == type) {
            items[i] = items[--size];
        }
    }
    player->inventory.size = size;
}

bool player_contains_item(struct Player player, enum ItemType type) {
    const int size = player.inventory.size;
    for (int i = 0; i < size; i++) {
        struct Item item = ((struct Item*)player.inventory.data)[i];
        if (item.type == type) {
            return true;
        }
    }
    return false;
}


struct Player player = {
    .maxHealth = 100,
    .health = 100
};