#include "hud.h"

#include <stddef.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "player/player.h"
#include "util/util.h"

#define BUTTON_FONT_SIZE 8
#define BUTTON_PADDING_X 6
#define BUTTON_PADDING_Y 4
#define BUTTON_SPACING   8

#define BUTTON_BACKGROUND CLITERAL(Color){ 76, 175, 80, 255 }
#define BUTTON_BORDER_BACKGROUND CLITERAL(Color){ 65, 147, 67, 255 }

#define BUTTON_BACKGROUND_HOVER CLITERAL(Color){ 95, 216, 99, 255 }
#define BUTTON_BORDER_BACKGROUND_HOVER CLITERAL(Color){ 76, 141, 55, 255 }

#define BUTTON_BACKGROUND_PRESS CLITERAL(Color){ 95, 216, 99, 255 }
#define BUTTON_BORDER_BACKGROUND_PRESS CLITERAL(Color){ 104, 237, 106, 255 }

#define DIALOGUE_FONT_SIZE 12
#define DIALOGUE_SPACING 3

void hud_set_flag(struct Hud *hud, int flag) {
    hud->option_flags |= flag;
}

void hud_unset_flag(struct Hud *hud, int flag) {
    hud->option_flags &= ~flag;
}

bool hud_has_flag(const struct Hud *hud, int flag) {
    return hud->option_flags & flag;
}

void buttons_tick(const struct Scene *scene, const struct Hud *hud);
void buttons_render(const struct Hud *hud);
void items_render(const struct Hud *hud);

void hud_update(struct Scene *scene, const struct Hud *hud) {
    if (!hud_has_flag(hud, RENDER_HUD)) {
        return;
    }

    if (hud_has_flag(hud, RENDER_BUTTONS) && hud->buttons.size > 0) {
        buttons_tick(scene, hud);
    }
}

void hud_draw(const struct Hud *hud) {
    if (!hud_has_flag(hud, RENDER_HUD)) {
        return;
    }

    if (hud_has_flag(hud, RENDER_BUTTONS) && hud->buttons.size > 0) {
        buttons_render(hud);
    }

    if (hud_has_flag(hud, RENDER_ITEMS)) {
        items_render(hud);
    }

    if (hud_has_flag(hud, RENDER_HEALTH)) {
        const int barWidth = 100;
        const int barHeight = 12;
        const int x = 8;
        const int y = SCREEN_HEIGHT - 8 - barHeight;

        DrawRectangle(x, y, barWidth, barHeight, GRAY);
        DrawRectangle(x, y, (int)(player.health / player.maxHealth * (float) barWidth), barHeight, GREEN);
        DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
    }


    if (hud->dialogue.size > 0) {
        for (int i = 0; i < hud->dialogue.size; i++) {
            const char* text = ((char**) hud->dialogue.data)[i];

            const int width = MeasureText(text, 8);
            const int y = SCREEN_HEIGHT - (BUTTON_FONT_SIZE + BUTTON_PADDING_Y * 2) - ((hud->dialogue.size - i) * (DIALOGUE_FONT_SIZE + DIALOGUE_SPACING)) - BUTTON_SPACING;
            DrawOutlinedText(text, (SCREEN_WIDTH / 2) - (width / 2), y, DIALOGUE_FONT_SIZE, WHITE, 1, BLACK);
        }
    }
}

void buttons_tick(const struct Scene *scene, const struct Hud *hud) {
    const int middle = SCREEN_WIDTH / 2;
    const Vector2 mousePos = GetScaledMousePos();

    int button_count = hud->buttons.size;
    struct Button* buttons = hud->buttons.data;

    if (button_count > 0) {
        int *widths = malloc(button_count * sizeof(int));

        int totalWidth = 0;
        for (int i = 0; i < button_count; i++) {
            if (buttons[i].hidden) {
                widths[i] = 0;
                continue;
            }
            const int textWidth = MeasureText(buttons[i].text, BUTTON_FONT_SIZE);
            widths[i] = textWidth + BUTTON_PADDING_X * 2;
            totalWidth += widths[i];
        }
        totalWidth += (button_count - 1) * BUTTON_SPACING;

        int startX = middle - totalWidth / 2;
        const int y = SCREEN_HEIGHT - (BUTTON_FONT_SIZE + BUTTON_PADDING_Y * 2);

        for (int i = 0; i < button_count; i++) {
            const int width = widths[i];
            if (width == 0) {
                continue;
            }
            const Rectangle rect = (Rectangle){
                (float) startX, (float) y - BUTTON_PADDING_Y, (float) width,
                (float) BUTTON_FONT_SIZE + BUTTON_PADDING_Y * 2
            };

            struct Button *btn = buttons + i;
            btn->hover = CheckCollisionPointRec(mousePos, rect);

            if (btn->hover) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && btn->on_click != NULL) {
                    btn->on_click((struct Scene *) scene, (struct Hud *) hud);
                }

                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    btn->pressed = true;
                } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                    btn->pressed = false;
                }
            } else if (btn->pressed && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                btn->pressed = false;
            }

            startX += width + BUTTON_SPACING;
        }
        free(widths);
    }
}

void buttons_render(const struct Hud *hud) {
    int button_count = hud->buttons.size;
    struct Button* buttons = hud->buttons.data;

    if (button_count > 0) {
        const int middle = SCREEN_WIDTH / 2;
        int *widths = malloc(button_count * sizeof(int));

        int totalWidth = 0;
        for (int i = 0; i < button_count; i++) {
            if (buttons[i].hidden) {
                widths[i] = 0;
                continue;
            }
            const int textWidth = MeasureText(buttons[i].text, BUTTON_FONT_SIZE);
            widths[i] = textWidth + BUTTON_PADDING_X * 2;
            totalWidth += widths[i];
        }
        totalWidth += (button_count - 1) * BUTTON_SPACING;

        int startX = middle - totalWidth / 2;
        const int y = SCREEN_HEIGHT - (BUTTON_FONT_SIZE + BUTTON_PADDING_Y * 2);

        for (int i = 0; i < button_count; i++) {
            const int width = widths[i];
            if (width == 0) {
                continue;
            }
            const Rectangle rect = (Rectangle){
                (float) startX, (float) y - BUTTON_PADDING_Y, (float) width,
                (float) BUTTON_FONT_SIZE + BUTTON_PADDING_Y * 2
            };

            if (buttons[i].pressed) {
                DrawRectangleRec(rect, BUTTON_BACKGROUND_PRESS);
                DrawRectangleLinesEx(rect, 2, BUTTON_BORDER_BACKGROUND_PRESS);
            } else if (buttons[i].hover) {
                DrawRectangleRec(rect, BUTTON_BACKGROUND_HOVER);
                DrawRectangleLinesEx(rect, 2, BUTTON_BORDER_BACKGROUND_HOVER);
            } else {
                DrawRectangleRec(rect, BUTTON_BACKGROUND);
                DrawRectangleLinesEx(rect, 2, BUTTON_BORDER_BACKGROUND);
            }

            const int textWidth = MeasureText(buttons[i].text, BUTTON_FONT_SIZE);
            const int textX = startX + (width - textWidth) / 2;
            const int textY = y - (BUTTON_FONT_SIZE / 2) + (BUTTON_PADDING_Y);

            DrawText(buttons[i].text, textX, textY, BUTTON_FONT_SIZE, BLACK);

            startX += width + BUTTON_SPACING;
        }
        free(widths);
    }
}

void items_render(const struct Hud *hud) {
    const int size = player.inventory.size;

    int offset_x = 8;

    for (int i = 0; i < size; i++) {
        const int offset_y = 8;
        const struct Item item = ((struct Item*) player.inventory.data)[i];

        DrawTexture(*item.texture, offset_x, offset_y, WHITE);

        offset_x += 8 + item.texture->width;
    }
}

void hud_hide_dialogue(struct Hud *hud) {
    hud->dialogue.size = 0;
}

void hud_set_dialogue(struct Hud *hud, char* text) {
    hud_set_dialogue_multi(hud, (char*[]) {text}, 1);
}

void hud_set_dialogue_multi(struct Hud *hud, char* text[], int lines) {
    array_allocate(&hud->dialogue, sizeof(char*), lines);

    for (int i = 0; i < lines; i++) {
        //*(char**)(hud->dialogue.data + (i * sizeof(char*))) = text[i];
        ((char**)hud->dialogue.data)[i] = text[i];
    }
    hud->dialogue.size = lines;
}