#include "items.h"
#include <raylib.h>

Texture2D item_texture_bear_tooth;
Texture2D item_texture_cloak;
Texture2D item_texture_coin;
Texture2D item_texture_golem_rock;
Texture2D item_texture_heal_potion;
Texture2D item_texture_strength_potion;
Texture2D item_texture_sword;

void load_items() {
    item_texture_bear_tooth = LoadTexture("assets/textures/items/bear_tooth.png");
    item_texture_cloak = LoadTexture("assets/textures/items/cloak.png");
    item_texture_coin = LoadTexture("assets/textures/items/coin.png");
    item_texture_golem_rock = LoadTexture("assets/textures/items/golem_rock.png");
    item_texture_heal_potion = LoadTexture("assets/textures/items/heal_potion.png");
    item_texture_strength_potion = LoadTexture("assets/textures/items/strength_potion.png");
    item_texture_sword = LoadTexture("assets/textures/items/sword.png");
}

 void unload_items() {
    UnloadTexture(item_texture_bear_tooth);
    UnloadTexture(item_texture_cloak);
    UnloadTexture(item_texture_coin);
    UnloadTexture(item_texture_golem_rock);
    UnloadTexture(item_texture_heal_potion);
    UnloadTexture(item_texture_strength_potion);
    UnloadTexture(item_texture_sword);
}
