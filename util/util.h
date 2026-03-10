#ifndef TESTRAYLIB_RAYLIB_UTIL_H
#define TESTRAYLIB_RAYLIB_UTIL_H
#include <raylib.h>

float getWidthScale();
float getHeightScale();
Vector2 getScaledMousePos();
Vector2 getScaledMousePosOffset(Vector2 offset);

void DrawOutlinedText(const char *text, int posX, int posY, int fontSize, Color color, int outlineSize, Color outlineColor);

#endif //TESTRAYLIB_RAYLIB_UTIL_H