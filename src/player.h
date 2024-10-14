#ifndef PLAYER_H
#define PLAYER_H

#include "util.h"

typedef struct {
    vec2f pos;
} player_t;

extern player_t player;

void player_update(float dt);

#endif // PLAYER_H
