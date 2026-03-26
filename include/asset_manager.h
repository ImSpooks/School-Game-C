#ifndef SCHOOLGAMEC_ASSET_MANAGER_H
#define SCHOOLGAMEC_ASSET_MANAGER_H

#include <raylib.h>



struct Assets {
    // -------------------------------
    // Textures
    // -------------------------------

    Texture2D empty_texture;
    Music empty_music;

    // levels
    Texture2D texture_level_start;
    Texture2D texture_level_forest1;
    Texture2D texture_level_forest2_bear;
    Texture2D texture_level_forest2_nobear;
    Texture2D texture_level_forest3;
    Texture2D texture_level_village1;
    Texture2D texture_level_village2;
    Texture2D texture_level_village3;
    Texture2D texture_level_village4;
    Texture2D texture_level_wizard1;
    Texture2D texture_level_wizard2;
    Texture2D texture_level_wizard3;
    
    //items
    Texture2D texture_item_bear_tooth;
    Texture2D texture_item_cloak;
    Texture2D texture_item_coin;
    Texture2D texture_item_golem_rock;
    Texture2D texture_item_heal_potion;
    Texture2D texture_item_strength_potion;
    Texture2D texture_item_sword;

    // enemies
    Texture2D texture_enemy_flowey;
    Texture2D texture_enemy_bear;
    Texture2D texture_enemy_golem;
    Texture2D texture_enemy_wizard;
    Texture2D texture_enemy_hawk;
    Texture2D texture_enemy_nick; // hidden boss
    Texture2D texture_enemy_renoir; // hidden boss

    // projectiles
    Texture2D texture_projectile_flowey;
    Texture2D texture_projectile_bear;

    // ui
    Texture2D texture_ui_damage_bar;
    Texture2D texture_ui_damage_slice;
    Texture2D texture_ui_damage_slider;
    Texture2D texture_ui_soul;
    Texture2D texture_ui_soul_broken;

    // -------------------------------
    // Music
    // -------------------------------

    // level
    Music music_forest;
    Music music_village;

    // enemy
    Music music_flowey;
    Music music_bear;
    Music music_golem_intro;
    Music music_golem_loop;
    Music music_wizard;
    Music music_hawk_intro;
    Music music_hawk_loop;
    Music music_nick;
    Music music_renoir;

    // sfx
    Sound sfx_attack;
    Sound sfx_damage_enemy;
    Sound sfx_damage_take;
    Sound sfx_victory;
};

void assets_load();
void assets_unload();

extern struct Assets assets;

#endif //SCHOOLGAMEC_ASSET_MANAGER_H