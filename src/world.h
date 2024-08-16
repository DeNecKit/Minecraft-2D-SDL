#ifndef WORLD_H
#define WORLD_H

#include <SDL.h>

#include "window.h"
#include "block.h"
#include "util.h"
#include "player.h"

#define BLOCK_SIZE 50
#define WORLD_WIDTH 1000
#define WORLD_HEIGHT 256
#define WORLD_SIZE (WORLD_WIDTH * WORLD_HEIGHT)
#define GRASS_MIN 185
#define GRASS_MAX 200
#define GRASS_RANGE (GRASS_MAX - GRASS_MIN)
#define DIRT_MIN 3
#define DIRT_MAX 7
#define DIRT_RANGE (DIRT_MAX - DIRT_MIN)

typedef block_t world_t[WORLD_SIZE];

void generate_world(world_t *world, u64 seed);
void render_world(world_t *world,
                  SDL_Renderer *renderer, player_t *player);

extern SDL_Texture *texture_blocks;

#endif // WORLD_H
