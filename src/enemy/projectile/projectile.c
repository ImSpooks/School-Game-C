#include "enemy/projectile/projectile.h"

#include <math.h>
#include <raymath.h>
#include <stdlib.h>

#include "asset_manager.h"
#include "player/player.h"

void projectile_damage_player(struct Projectile* projectile) {
    if (player.invincibility_timer > 0) {
        return;
    }

    // random from 2 to 4
    float damage = 2.0f + (float) ((double) rand() / (double) RAND_MAX * 2.0);

    if (projectile->true_damage)
        damage = damage * projectile->damage;
    else
        damage = damage * projectile->damage / sqrtf(powf(player_get_defence(player), 1.75f));

    player.health -= damage;
    player.invincibility_timer = 0.5f;
    PlaySound(assets.sfx_damage_take);

    // for now
    player.health = 0.0f;
}

void projectile_heal_player(struct Projectile* projectile) {
    player.health = Clamp(player.health + projectile->damage, 0, player.maxHealth);
    PlaySound(assets.sfx_heal);
    projectile_despawn(projectile);
}

void projectile_despawn(struct Projectile *projectile) {
    projectile->lifespan = 100;
}