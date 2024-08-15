#ifndef BLOCK_H
#define BLOCK_H

#include <SDL.h>

typedef enum {
    BLOCK_NONE,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE,
    BLOCK_COUNT
} block_type_t;

typedef struct {
    block_type_t type;
    int x, y;
} block_t;

void block_init();

extern SDL_Rect block_rects[BLOCK_COUNT];

#endif // BLOCK_H
