#ifndef BLOCK_H
#define BLOCK_H

typedef enum {
    BLOCK_NONE,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE
} block_type_t;

typedef struct {
    block_type_t type;
    int x, y;
} block_t;

#endif // BLOCK_H
