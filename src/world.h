#ifndef WORLD_H
#define WORLD_H

#include <SDL.h>

#include "window.h"
#include "block.h"
#include "chunk.h"
#include "util.h"
#include "player.h"

#define WORLD_CHUNKS_WIDTH \
    (max(WINDOW_WIDTH/BLOCK_SIZE*3/2/CHUNK_WIDTH/3*3, 3))
#define WORLD_CHUNKS_HEIGHT \
    (max(WINDOW_HEIGHT/BLOCK_SIZE*3/2/CHUNK_HEIGHT/3*3, 3))
#define WORLD_CHUNKS_NUM (WORLD_CHUNKS_WIDTH * WORLD_CHUNKS_HEIGHT)
#define WORLD_HEIGHT 256
#define BLOCK_SIZE 50
#define GRASS_MIN 185
#define GRASS_MAX 200
#define GRASS_RANGE (GRASS_MAX - GRASS_MIN)
#define DIRT_MIN 3
#define DIRT_MAX 7
#define DIRT_RANGE (DIRT_MAX - DIRT_MIN)

typedef struct {
    chunk_t chunks[WORLD_CHUNKS_NUM];
} world_t;

void generate_world(world_t *world, u64 seed);
void render_world(world_t *world, SDL_Renderer *renderer);

extern SDL_Texture *texture_blocks;

#endif // WORLD_H
