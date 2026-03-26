#ifndef TESTRAYLIB_RAYLIB_UTIL_H
#define TESTRAYLIB_RAYLIB_UTIL_H
#include <raylib.h>

// raylib type functions
float GetWidthScale();
float GetHeightScale();
Vector2 GetScaledMousePos();

void DrawOutlinedText(const char *text, int posX, int posY, int fontSize, Color color, int outlineSize, Color outlineColor);

#endif //TESTRAYLIB_RAYLIB_UTIL_H