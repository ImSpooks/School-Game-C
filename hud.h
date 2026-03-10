#ifndef TESTRAYLIB_HUD_H
#define TESTRAYLIB_HUD_H
#include <raylib.h>

struct HUDOptions {
    bool render_hud;
    bool render_background;
    bool render_buttons;
};

typedef struct HUD {
    RenderTexture2D texture;
    Rectangle offset;
    struct HUDOptions options;
} HUD;

extern HUD hud;

typedef struct Button {
    char* text;
    void (*onClick)();
    bool hover;
    bool pressed;
} Button;

void drawHud(const RenderTexture2D* texture);
void updateHud(const RenderTexture2D* texture);

void setButtons(Button* buttons, int count);

#endif //TESTRAYLIB_HUD_H