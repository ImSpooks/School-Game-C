#include "hud.h"

#include <stddef.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "vec.h"
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


Button *buttons = NULL;
int button_count = 0;

struct Dialogue dialogue;

void tickButtons(const RenderTexture2D *texture);

void renderButtons(const RenderTexture2D *texture);
void renderItems(const RenderTexture2D *texture);

void updateHud(const RenderTexture2D *texture) {
    if (!hud.render_hud) {
        return;
    }

    if (hud.render_buttons && buttons != NULL && button_count > 0) {
        tickButtons(texture);
    }
}

void drawHud(const RenderTexture2D *texture) {
    if (!hud.render_hud) {
        return;
    }

    if (hud.render_buttons && buttons != NULL && button_count > 0) {
        renderButtons(texture);
    }

    if (hud.render_items) {
        renderItems(texture);
    }

    if (hud.render_health) {
        const int barWidth = 100;
        const int barHeight = 12;
        const int x = 8;
        const int y = SCREEN_HEIGHT - 8 - barHeight;

        DrawRectangle(x, y, barWidth, barHeight, GRAY);
        DrawRectangle(x, y, (int)((player.health / (float)player.maxHealth) * barWidth), barHeight, GREEN);
        DrawRectangleLines(x, y, barWidth, barHeight, BLACK);
    }

    if (dialogue.lines != NULL) {
        for (int i = 0; i < dialogue.lineCount; i++) {
            const char* text = *(dialogue.lines + i);
            const int width = MeasureText(text, 8);
            const int y = SCREEN_HEIGHT - (BUTTON_FONT_SIZE + BUTTON_PADDING_Y * 2) - ((dialogue.lineCount - i) * (DIALOGUE_FONT_SIZE + DIALOGUE_SPACING)) - BUTTON_SPACING;
            DrawOutlinedText(text, (SCREEN_WIDTH / 2) - (width / 2), y, DIALOGUE_FONT_SIZE, WHITE, 1, BLACK);
        }
    }
}

void tickButtons(const RenderTexture2D *texture) {
    const int middle = SCREEN_WIDTH / 2;
    const Vector2 mousePos = getScaledMousePos();

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

            Button *btn = buttons + i;
            btn->hover = CheckCollisionPointRec(mousePos, rect);

            if (btn->hover) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && btn->onClick != NULL) {
                    btn->onClick();
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

void renderButtons(const RenderTexture2D *texture) {
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

void renderItems(const RenderTexture2D *texture) {
    size_t size = vector_size(player.inventory);

    int offsetX = 8;
    int offsetY = 8;

    const int outline = 2;

    for (int i = 0; i < size; i++) {
        Item* item = player.inventory + i;

        DrawTexture(*item->texture, offsetX, offsetY, WHITE);

        offsetX += 8 + item->texture->width;
    }
}

void setButtons(Button *btns, int count) {
    buttons = btns;
    button_count = count;
}
