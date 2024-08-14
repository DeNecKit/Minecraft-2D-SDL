#ifndef WORLD_H
#define WORLD_H

#include "window.h"
#include "block.h"

#define BLOCK_SIZE 16
#define WORLD_WIDTH (WINDOW_WIDTH / BLOCK_SIZE)
#define WORLD_HEIGHT (WINDOW_HEIGHT / BLOCK_SIZE)
#define WORLD_SIZE (WORLD_WIDTH * WORLD_HEIGHT)
#define GRASS_MIN 10
#define GRASS_MAX 20
#define DIRT_MIN 3
#define DIRT_MAX 7

typedef block_t world_t[WORLD_SIZE];

void generate_world(world_t *world, unsigned char seed);

#endif // WORLD_H
