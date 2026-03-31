#include "asset_manager.h"

#include <rlgl.h>

struct Assets assets;

void assets_load() {
    // -------------------------------
    // Textures
    // -------------------------------

    // levels
    assets.texture_level_start = LoadTexture("assets/textures/levels/start.png");
    assets.texture_level_forest1 = LoadTexture("assets/textures/levels/forest1.png");
    assets.texture_level_forest2_bear = LoadTexture("assets/textures/levels/forest2_bear.png");
    assets.texture_level_forest2_nobear = LoadTexture("assets/textures/levels/forest2_nobear.png");
    assets.texture_level_forest3 = LoadTexture("assets/textures/levels/forest3.png");
    assets.texture_level_village1 = LoadTexture("assets/textures/levels/village1.png");
    assets.texture_level_village2 = LoadTexture("assets/textures/levels/village2.png");
    assets.texture_level_village3 = LoadTexture("assets/textures/levels/village3.png");
    assets.texture_level_village4 = LoadTexture("assets/textures/levels/village4.png");
    assets.texture_level_wizard1 = LoadTexture("assets/textures/levels/wizard1.png");
    assets.texture_level_wizard2 = LoadTexture("assets/textures/levels/wizard2.png");
    assets.texture_level_wizard3 = LoadTexture("assets/textures/levels/wizard3.png");
    
    //items
    assets.texture_item_bear_tooth = LoadTexture("assets/textures/items/bear_tooth.png");
    assets.texture_item_cloak = LoadTexture("assets/textures/items/cloak.png");
    assets.texture_item_coin = LoadTexture("assets/textures/items/coin.png");
    assets.texture_item_golem_rock = LoadTexture("assets/textures/items/golem_rock.png");
    assets.texture_item_heal_potion = LoadTexture("assets/textures/items/heal_potion.png");
    assets.texture_item_strength_potion = LoadTexture("assets/textures/items/strength_potion.png");
    assets.texture_item_sword = LoadTexture("assets/textures/items/sword.png");

    // enemies
    assets.texture_enemy_flowey = LoadTexture("assets/textures/enemies/flowey.png");
    assets.texture_enemy_bear = LoadTexture("assets/textures/enemies/bear.png");
    assets.texture_enemy_golem = LoadTexture("assets/textures/enemies/golem.png");
    assets.texture_enemy_wizard = LoadTexture("assets/textures/enemies/wizard.png");
    assets.texture_enemy_hawk = LoadTexture("assets/textures/enemies/hawk.png");
    assets.texture_enemy_nick = LoadTexture("assets/textures/enemies/nick.png"); // hidden boss
    assets.texture_enemy_renoir = LoadTexture("assets/textures/enemies/renoir.png"); // hidden boss

    // projectiles
    assets.texture_projectile_flowey = LoadTexture("assets/textures/enemies/projectile/flowey.png");
    assets.texture_projectile_bear = LoadTexture("assets/textures/enemies/projectile/bear.png");
    assets.texture_projectile_golem = LoadTexture("assets/textures/enemies/projectile/golem.png");
    assets.texture_projectile_wizard_tornado = LoadTexture("assets/textures/enemies/projectile/wizard_tornado.png");
    assets.texture_projectile_wizard_orb = LoadTexture("assets/textures/enemies/projectile/wizard_orb.png");
    assets.texture_projectile_wizard_heal_orb = LoadTexture("assets/textures/enemies/projectile/wizard_heal_orb.png");

    // ui
    assets.texture_ui_damage_bar = LoadTexture("assets/textures/ui/damage_bar.png");
    assets.texture_ui_damage_slice = LoadTexture("assets/textures/ui/damage_slice.png");
    assets.texture_ui_damage_slider = LoadTexture("assets/textures/ui/damage_slider.png");
    assets.texture_ui_soul = LoadTexture("assets/textures/ui/soul.png");
    assets.texture_ui_soul_broken = LoadTexture("assets/textures/ui/soul_broken.png");

    // -------------------------------
    // Music
    // -------------------------------

    // level
    assets.music_forest = LoadMusicStream("assets/music/levels/forest.ogg");
    assets.music_village = LoadMusicStream("assets/music/levels/village.ogg");

    // enemy
    assets.music_flowey = LoadMusicStream("assets/music/enemies/flowey.ogg");
    assets.music_bear = LoadMusicStream("assets/music/enemies/bear.ogg");
    assets.music_golem_intro = LoadMusicStream("assets/music/enemies/golem_intro.ogg");
    assets.music_golem_loop = LoadMusicStream("assets/music/enemies/golem_loop.ogg");
    assets.music_wizard = LoadMusicStream("assets/music/enemies/wizard.ogg");
    assets.music_hawk_intro = LoadMusicStream("assets/music/enemies/hawk_intro.ogg");
    assets.music_hawk_loop = LoadMusicStream("assets/music/enemies/hawk_loop.ogg");
    assets.music_nick = LoadMusicStream("assets/music/enemies/nick.ogg");
    //assets.music_renoir = LoadMusicStream("assets/music/enemies/renoir.ogg");


    // sfx
    assets.sfx_attack = LoadSound("assets/sfx/attack.wav");
    assets.sfx_damage_enemy = LoadSound("assets/sfx/damage_enemy.wav");
    assets.sfx_damage_take = LoadSound("assets/sfx/damage_take.wav");
    assets.sfx_victory = LoadSound("assets/sfx/victory.wav");
    assets.sfx_heal = LoadSound("assets/sfx/heal.wav");
}

void assets_unload() {
    // -------------------------------
    // Textures
    // -------------------------------

    // levels
    UnloadTexture(assets.texture_level_start);
    UnloadTexture(assets.texture_level_forest1);
    UnloadTexture(assets.texture_level_forest2_bear);
    UnloadTexture(assets.texture_level_forest2_nobear);
    UnloadTexture(assets.texture_level_forest3);
    UnloadTexture(assets.texture_level_village1);
    UnloadTexture(assets.texture_level_village2);
    UnloadTexture(assets.texture_level_village3);
    UnloadTexture(assets.texture_level_village4);
    UnloadTexture(assets.texture_level_wizard1);
    UnloadTexture(assets.texture_level_wizard2);
    UnloadTexture(assets.texture_level_wizard3);

    //items
    UnloadTexture(assets.texture_item_bear_tooth);
    UnloadTexture(assets.texture_item_cloak);
    UnloadTexture(assets.texture_item_coin);
    UnloadTexture(assets.texture_item_golem_rock);
    UnloadTexture(assets.texture_item_heal_potion);
    UnloadTexture(assets.texture_item_strength_potion);
    UnloadTexture(assets.texture_item_sword);

    // enemies
    UnloadTexture(assets.texture_enemy_flowey);
    UnloadTexture(assets.texture_enemy_bear);
    UnloadTexture(assets.texture_enemy_golem);
    UnloadTexture(assets.texture_enemy_wizard);
    UnloadTexture(assets.texture_enemy_hawk);
    UnloadTexture(assets.texture_enemy_nick); // hidden boss
    UnloadTexture(assets.texture_enemy_renoir); // hidden boss

    // projectiles
    UnloadTexture(assets.texture_projectile_flowey);
    UnloadTexture(assets.texture_projectile_bear);
    UnloadTexture(assets.texture_projectile_golem);
    UnloadTexture(assets.texture_projectile_wizard_tornado);
    UnloadTexture(assets.texture_projectile_wizard_orb);
    UnloadTexture(assets.texture_projectile_wizard_heal_orb);


    // ui
    UnloadTexture(assets.texture_ui_damage_bar);
    UnloadTexture(assets.texture_ui_damage_slice);
    UnloadTexture(assets.texture_ui_damage_slider);
    UnloadTexture(assets.texture_ui_soul);
    UnloadTexture(assets.texture_ui_soul_broken);

    // -------------------------------
    // Music
    // -------------------------------

    // level
    UnloadMusicStream(assets.music_forest);
    UnloadMusicStream(assets.music_village);

    // enemy
    UnloadMusicStream(assets.music_flowey);
    UnloadMusicStream(assets.music_bear);
    UnloadMusicStream(assets.music_golem_intro);
    UnloadMusicStream(assets.music_golem_loop);
    UnloadMusicStream(assets.music_wizard);
    UnloadMusicStream(assets.music_hawk_intro);
    UnloadMusicStream(assets.music_hawk_loop);
    UnloadMusicStream(assets.music_nick);
    //UnloadMusicStream(assets.music_renoir);

    // sfx
    UnloadSound(assets.sfx_attack);
    UnloadSound(assets.sfx_damage_enemy);
    UnloadSound(assets.sfx_damage_take);
    UnloadSound(assets.sfx_victory);
    UnloadSound(assets.sfx_heal);
}
