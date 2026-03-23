#include "../../../include/enemy/projectile/projectile.h"

#include <math.h>
#include <stdlib.h>

#include "player/player.h"


void collides_with_player(Projectile* projectile) {

}

void damage_player(Projectile* projectile, bool ignore_armor) {
    // random from 2 to 4
    float damage = 2.0f + ((float)rand() / (float) RAND_MAX * 2.0f);

    if (!ignore_armor) {
        damage = damage * projectile->damage / powf(get_defence_stat(), 1.75f);
    } else {
        damage = damage * projectile->damage;
    }

    player.health -= damage;
    player.invincibility_timer = 0.5f;
}