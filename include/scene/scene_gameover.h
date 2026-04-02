#ifndef SCHOOLGAMEC_SCENE_GAMEOVER_H
#define SCHOOLGAMEC_SCENE_GAMEOVER_H


#define GAME_OVER_TEXT_1 ((char*) "You cannot give up\njust yet...")
#define GAME_OVER_TEXT_2 ((char*)"Human!\n%Stay determined...")

#include "scene/scene.h"

enum GameOverState {
    INIT,
    SOUL_BREAK,
    SOUL_SHATTER,
    ASGORE,
    FINISH,
};

struct GameOverData {
    float timer;

    enum GameOverState state;
    struct Array soul_particles;

    float letters;
    int   active_line;
    float wait_timer;
    bool queue_next_line;

};

void gameover_scene_load(struct Scene *scene, struct Hud *hud);
void gameover_scene_unload(struct Scene *scene, struct Hud *hud);
void gameover_scene_update(struct Scene *scene, struct Hud *hud);
void gameover_scene_draw(void* scene_data);

#endif //SCHOOLGAMEC_SCENE_GAMEOVER_H