#include "asset_manager.h"
#include "level/level_registry.h"
#include "scene/scene_level.h"
#include "player/player.h"
#include "scene/scene_battle.h"

void forest3_pay_pirate(struct Scene *scene, struct Hud *hud);
void forest3_to_wizard(struct Scene *scene, struct Hud *hud);
void forest3_go_back(struct Scene *scene, struct Hud *hud);

void level_forest3_enter(struct Hud *hud) {
    array_allocate(&hud->buttons, sizeof(struct Button), 2);

    if (player.flags.paid_pirate) {
        hud_set_dialogue(hud, "Pirate: What are ya doin' here pal?");

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Go to the wizard",
            .on_click = forest3_to_wizard
        };
    } else {
        if (player_contains_item(player, COIN)) {
            hud_set_dialogue_multi(hud, (char*[]) {
                "Pirate: Hello pal, are you trying to go on?",
                "You can only pass us by paying us a",
                "golden coin, so what are you waiting for?"
            }, 3);
        } else {
            hud_set_dialogue_multi(hud, (char*[]) {
                "Pirate: Hello pal, are you trying to go on?",
                "You can only pass us by paying us atleast",
                "10 gold, so what are you waiting for?",
                " ",
                "What? You dont have any gold? You might be able to",
                "sell some stuff at the merchant in the nearest village."
            }, 6);
        }

        ((struct Button*)hud->buttons.data)[0] = (struct Button) {
            .text = "Pay the pirate",
            .on_click = forest3_pay_pirate
        };
    }

    ((struct Button*)hud->buttons.data)[1] = (struct Button) {
        .text = "Go back",
        .on_click = forest3_go_back
    };

    hud->buttons.size = 2;
}

void level_forest3_leave(struct Hud *hud) {

}

void forest3_pay_pirate(struct Scene *scene, struct Hud *hud) {
    if (player_contains_item(player, COIN)) {
        player.flags.paid_pirate = true;

        player_remove_item(&player, COIN);

        hud_set_dialogue_multi(hud, (char*[]) {
            "Pirate: That is what I call a good deal, but take this.",
            "It will protect you against some monsters.",
            "And I'll give you this potion as well",
            "it'll boost your attack."
        }, 4);

        player_add_item(&player, (struct Item) {
            .type = DEFENCE,
            .texture = &assets.texture_item_cloak,
            .value = 18
        });

        player_add_item(&player, (struct Item) {
            .type = ATTACK,
            .texture = &assets.texture_item_strength_potion,
            .value = 17
        });

        set_level_fade(scene, &level_registry.levelForest3, hud);
    } else {
        hud_set_dialogue(hud, "You do not have a golden coin.");
    }
}

void forest3_to_wizard(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelWizard1, hud);
}

void forest3_go_back(struct Scene *scene, struct Hud *hud) {
    set_level_fade(scene, &level_registry.levelForest2, hud);
}