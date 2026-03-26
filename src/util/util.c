#include "util/util.h"

#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>

#include "globals.h"

float GetWidthScale() {
    const float virtualWidthRatio = (float)GetScreenWidth() / (float)SCREEN_WIDTH;
    return virtualWidthRatio;
}

float GetHeightScale() {
    const float virtualHeightRatio = (float)GetScreenHeight() / (float)SCREEN_HEIGHT;
    return virtualHeightRatio;
}

Vector2 GetScaledMousePos() {
    float mouseX = (float) GetMouseX() / GetWidthScale();
    float mouseY = (float) GetMouseY() / GetHeightScale();
    return (Vector2){mouseX, mouseY};
}


void DrawOutlinedText(const char *text, int posX, int posY, int fontSize, Color color, int outlineSize, Color outlineColor) {
    if (text == NULL) return;
    if (outlineSize <= 0) {
        DrawText(text, posX, posY, fontSize, color);
        return;
    }

    int r = outlineSize;
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx == 0 && dy == 0) continue;
            DrawText(text, posX + dx, posY + dy, fontSize, outlineColor);
        }
    }

    DrawText(text, posX, posY, fontSize, color);
}