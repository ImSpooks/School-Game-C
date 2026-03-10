#include "raylib_util.h"
#include <raylib.h>
#include <stddef.h>

void DrawOutlinedText(const char *text, int posX, int posY, int fontSize, Color color, int outlineSize, Color outlineColor) {
    if (text == NULL) return;
    if (outlineSize <= 0) {
        DrawText(text, posX, posY, fontSize, color);
        return;
    }

    int r = outlineSize;
    int r2 = r * r;
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx++) {
            if (dx == 0 && dy == 0) continue;
            if (dx*dx + dy*dy <= r2) {
                DrawText(text, posX + dx, posY + dy, fontSize, outlineColor);
            }
        }
    }

    DrawText(text, posX, posY, fontSize, color);
}