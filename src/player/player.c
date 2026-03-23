#include "player/player.h"

#include "vec.h"

Player player;

float get_inventory_stat(ItemType type) {
    float base = 1.0f;

    size_t size = vector_size(player.inventory);
    for (int i = 0; i < size; i++) {
        Item item = player.inventory[i];
        if (item.type == type) {
            base += item.value;
        }
    }
    return base;
}

float get_attack_stat() {
    return get_inventory_stat(ATTACK);
}

float get_defence_stat() {
    return get_inventory_stat(DEFENCE);
}

void remove_item_index(int index) {

}

void remove_item_type(ItemType type) {
    size_t size = vector_size(player.inventory);
    for (int i = 0; i < size; i++) {
        Item item = player.inventory[i];
        if (item.type == type) {
            vector_remove(player.inventory, i);
            i--;
            size--;
        }
    }
}

bool contains_item(ItemType type) {
    size_t size = vector_size(player.inventory);
    for (int i = 0; i < size; i++) {
        Item item = player.inventory[i];
        if (item.type == type) {
            return true;
        }
    }
    return false;
}