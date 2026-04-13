#include "scene/scene_credits.h"

#include <stdlib.h>
#include <string.h>

#include "globals.h"

#define SCROLL_SPEED 17
#define DEFAULT_TEXT_SIZE 17

void restart(struct Scene *scene, struct Hud *hud);

struct CreditsText {
    char *text;
    int font_size;
};

int credits_length = 0;
struct CreditsText *credits_text = (struct CreditsText[]){
    {"CREDITS", 60}, // h1
    {""},
    {""},
    {""},
    {"Adventure Game", 40}, // h2
    {""},
    {"Original game was made in plain JavaScript"},
    {"Recoded with C using Raylib"},
    {""},
    {"Note: I do not own any images/audio"},
    {"in this game."},
    {""},
    {"Music", 40}, // h2
    {""},
    {""},
    {""},
    {"Credits: Last Goodbye by Toby Fox"},
    {""},
    {""},
    {""},
    {"Boss Fights", 29}, // h3
    {""},
    {""},
    {"Flowey Fight: Your Best Friend by Toby Fox"},
    {""},
    {"Bear Fight: Bonetrousle by Toby Fox"},
    {""},
    {"Golem Fight: Bergentrückung"},
    {"       & ASGORE by Toby Fox"},
    {""},
    {"Wizard Fight: Heart Ache by Toby Fox"},
    {""},
    {"Giant Hawk Fight: Hopes and Dreams"},
    {"       & SAVE the World by Toby Fox"},
    {""},
    {""},
    {""},
    {"Secret Bosses", 29},
    {""},
    {""},
    {"Nick Fight: Pokemon Arceus Battle Theme by Luigigigas"},
    {""},
    {"Renoir Fight: Une vie à t'aimer by Lorien Testard"},
    {""},
    {""},
    {""},
    {"Overworld", 29}, // h3
    {""},
    {""},
    {"Forest Area: Snowy by Toby Fox"},
    {""},
    {"Village Area: Temmie Village by Toby Fox"},
    {""},
    {""},
    {""},
    {""},
    {"Special Thanks"}, // h2
    {""},
    {"Toby Fox, creator of Undertale & Deltarune"},
    {"   As \"inspiration\" for the battle system and music"},
    {""},
    {"My college"},
    {"   For giving me an assignment to make an \"Adventure Game\""},
    {""},
    {"My workplace"},
    {"    For making me have to work in C++ without prior experience"},
    {"    and giving me the freedom to learn C and improve my skills."},
    {""},
    {"And..."},
    {"... YOU, for playing this game :)"},
    {""},
    {""},
    {""},
    {""},
    {""},
    {""},
    {""},
    {""},
    {""},
    {""},
    {""},
    {""},
    {""},
    {"Thanks for playing!", 40}, // h1
    {"END"}
};

void credits_scene_load(struct Scene *scene, struct Hud *hud) {
    hud_unset_flag(hud, RENDER_ITEMS);
    hud->buttons.size = 0;

    if (!IsMusicStreamPlaying(*scene->music)) {
        PlayMusicStream(*scene->music);
    }

    if (credits_length == 0) {
        while (strcmp(credits_text[credits_length].text, "END") != 0) {
            credits_length++;
        }
    }

    // Make it so the timer stops when "Thanks for playing!" is centered on the screen
    struct CreditsData *data = scene->data;
    int y = SCREEN_HEIGHT;
    for (int i = 0; i < credits_length; i++) {
        const int font_size = credits_text[i].font_size > 0 ? credits_text[i].font_size : DEFAULT_TEXT_SIZE;
        if (strcmp(credits_text[i].text, "Thanks for playing!") == 0) {
            data->max_timer = ((float) y - ((float) SCREEN_HEIGHT / 2.0f - (float) font_size / 2.0f)) / (float)
                              SCROLL_SPEED;
            break;
        }
        y += font_size + 4;
    }

    array_allocate(&hud->buttons, sizeof(struct Button), 1);
    ((struct Button*) hud->buttons.data)[0] = (struct Button) {
        .text = "Back to title",
        .on_click = restart
    };
    hud->buttons.size = 0;
}

void credits_scene_unload(struct Scene *scene, struct Hud *hud) {
    if (IsMusicStreamPlaying(*scene->music)) {
        StopMusicStream(*scene->music);
    }
}

void credits_scene_update(struct Scene *scene, struct Hud *hud) {
    struct CreditsData *data = scene->data;
    data->timer += GetFrameTime();


    if (hud->buttons.size == 0 && data->max_timer > 0.0f && data->timer >= data->max_timer) {
        hud->buttons.size = 1;
    }
}

void credits_scene_draw(void *scene_data) {
    ClearBackground(BLACK);

    float timer     = ((struct CreditsData*) scene_data)->timer;
    float max_timer = ((struct CreditsData*) scene_data)->max_timer;
    int y = SCREEN_HEIGHT - (int) ((timer < max_timer ? timer : max_timer) * SCROLL_SPEED);

    for (int i = 0; i < credits_length; i++) {
        if (y > SCREEN_HEIGHT) {
            continue;
        }

        const struct CreditsText text = credits_text[i];
        const int font_size = text.font_size > 0 ? text.font_size : DEFAULT_TEXT_SIZE;

        if (y > -font_size) {
            const int width = MeasureText(text.text, font_size);
            const int x = (SCREEN_WIDTH - width) / 2;
            DrawText(text.text, x, y, font_size, WHITE);
        }

        y += font_size + 4;
    }
}

void restart(struct Scene *scene, struct Hud *hud) {
    reset_game();
    scene->request_scene_change.type = TITLE;
}