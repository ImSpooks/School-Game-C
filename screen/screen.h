#ifndef TESTRAYLIB_SCREEN_H
#define TESTRAYLIB_SCREEN_H
#include <raylib.h>

typedef struct Screen {
    void (*initialize)();
    void (*unload)();
    void (*update)(const RenderTexture2D*);
    void (*draw)(const RenderTexture2D*);
} Screen;

extern Screen titleScreen;
extern Screen gameScreen;
extern Screen battleScreen;

void setScreen(Screen* screen);
void setDialogue(char* text);

#endif //TESTRAYLIB_SCREEN_H