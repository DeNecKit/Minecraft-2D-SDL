#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    float x, y;
} player_t;

void player_update(player_t *player, float dt);

#endif // PLAYER_H
