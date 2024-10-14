#include "block.h"

void block_init()
{
    assert(BLOCK_COUNT <= 256);
    for (block_type_t block_type = BLOCK_NONE + 1;
         block_type < BLOCK_COUNT; block_type++) {
        block_rects[block_type] = (SDL_Rect) {
            .x = (block_type - 1) % 16 * 16,
            .y = (block_type - 1) / 16 * 16,
            .w = 16, .h = 16
        };
    }
}

SDL_Rect block_rects[BLOCK_COUNT] = { 0 };
