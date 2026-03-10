#include "hud.h"

#include <stddef.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/raylib_util.h"

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

Button *buttons = NULL;
int button_count = 0;

void tickButtons(const RenderTexture2D *texture);

void renderButtons(const RenderTexture2D *texture);

void updateHud(const RenderTexture2D *texture) {
    if (!hud.options.render_hud) {
        return;
    }

    if (hud.options.render_buttons && buttons != NULL && button_count > 0) {
        tickButtons(texture);
    }
}

void drawHud(const RenderTexture2D *texture) {
    if (!hud.options.render_hud) {
        return;
    }

    if (hud.options.render_buttons && buttons != NULL && button_count > 0) {
        renderButtons(texture);
    }
}

void tickButtons(const RenderTexture2D *texture) {
    const int middle = texture->texture.width / 2;
    const Vector2 mousePos = getScaledMousePosOffset((Vector2){hud.offset.x, hud.offset.y});

    int *widths = malloc(button_count * sizeof(int));

    int totalWidth = 0;
    for (int i = 0; i < button_count; i++) {
        const int textWidth = MeasureText(buttons[i].text, BUTTON_FONT_SIZE);
        widths[i] = textWidth + BUTTON_PADDING_X * 2;
        totalWidth += widths[i];
    }
    totalWidth += (button_count - 1) * BUTTON_SPACING;

    int startX = middle - totalWidth / 2;
    const int y = BUTTON_PADDING_Y * 2;

    for (int i = 0; i < button_count; i++) {
        const int width = widths[i];
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

void renderButtons(const RenderTexture2D *texture) {
    const int middle = texture->texture.width / 2;
    int *widths = malloc(button_count * sizeof(int));

    int totalWidth = 0;
    for (int i = 0; i < button_count; i++) {
        const int textWidth = MeasureText(buttons[i].text, BUTTON_FONT_SIZE);
        widths[i] = textWidth + BUTTON_PADDING_X * 2;
        totalWidth += widths[i];
    }
    totalWidth += (button_count - 1) * BUTTON_SPACING;

    int startX = middle - totalWidth / 2;
    const int y = BUTTON_PADDING_Y * 2;

    for (int i = 0; i < button_count; i++) {
        const int width = widths[i];
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

void setButtons(Button *btns, int count) {
    buttons = btns;
    button_count = count;
}
