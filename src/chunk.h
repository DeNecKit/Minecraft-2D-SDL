#ifndef CHUNK_H
#define CHUNK_H

#include "util.h"
#include "block.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_SIZE (CHUNK_WIDTH * CHUNK_HEIGHT)

typedef struct {
    vec2 pos;
    block_t blocks[CHUNK_SIZE];
    SDL_Texture *texture;
    bool is_updated;
} chunk_t;

extern bool chunk_borders;

void chunk_generate(chunk_t *chunk, u64 seed);
void chunk_render(chunk_t *chunk, SDL_Renderer *renderer);

#endif // CHUNK_H
