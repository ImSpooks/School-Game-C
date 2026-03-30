#ifndef TESTRAYLIB_SCENE_BATTLE_H
#define TESTRAYLIB_SCENE_BATTLE_H

#define CALCULATING_DAMAGE_LENGTH   1.5f
#define DEALING_DAMAGE_LENGTH       0.75f
#define RENDERING_DAMAGE_LENGTH     1.0f

#define BATTLE_BOUNDS (Rectangle) {(640.0f / 2.0f) - (192.0f / 2.0f), (360.0f / 3.0f * 2.0f) - (132.0f / 2.0f), 192.0f, 132.0f}
#define BATTLE_BOUNDS_MIDDLE (Vector2) {BATTLE_BOUNDS.x + BATTLE_BOUNDS.width / 2.0f, BATTLE_BOUNDS.y + BATTLE_BOUNDS.height / 2.0f}
#define BATTLE_BOUNDS_EXPAND_TIME 0.15f

#include "scene/scene.h"
#include "enemy/enemy.h"

enum BattleState {
    PLAYER_TURN,
    CALCULATING_DAMAGE,
    DEALING_DAMAGE,
    MOVE_HEALTHBAR,
    ENEMY,
    ENEMY_FINISH,
    FINISHED,
};

struct BattleContext {
    float timer;
    float last_damage;
    int enemy_rand_attack;
};

struct BattleData {
    enum BattleState state;
    struct BattleContext context;

    Enemy enemy;
    int turn;
    struct Array projectiles;
};

void battle_scene_load(struct Scene *scene, struct Hud *hud);
void battle_scene_unload(struct Scene *scene, struct Hud *hud);
void battle_scene_post_unload(struct Scene *scene, struct Hud *hud);
void battle_scene_update(struct Scene *scene, struct Hud *hud);
void battle_scene_draw(void* scene_data);

#endif //TESTRAYLIB_SCENE_BATTLE_H