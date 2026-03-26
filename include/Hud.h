#ifndef TESTRAYLIB_HUD_H
#define TESTRAYLIB_HUD_H

#include <raylib.h>
#include "util/array.h"

struct Hud;
struct Scene; // We exist somewhere

struct Button {
    char* text;
    void (*on_click)(struct Scene *scene, struct Hud *hud);
    bool hover;
    bool pressed;
    bool hidden;
};

enum HudFlags {
    RENDER_HUD = 1 << 0,
    RENDER_BACKGROUND = 1 << 1,
    RENDER_BUTTONS = 1 << 2,
    RENDER_ITEMS = 1 << 3,
    RENDER_HEALTH = 1 << 4
};

struct Hud {
    int option_flags;
    struct Array dialogue;
    struct Array buttons;
};

void hud_draw(const struct Hud *hud);
void hud_update(struct Scene *scene, const struct Hud *hud);

void hud_set_flag(struct Hud *hud, int flag);
void hud_unset_flag(struct Hud *hud, int flag);
bool hud_has_flag(const struct Hud *hud, int flag);

void hud_hide_dialogue(struct Hud *hud);
void hud_set_dialogue(struct Hud *hud, char* text);
void hud_set_dialogue_multi(struct Hud *hud, char* text[], int lines);


#endif //TESTRAYLIB_HUD_H