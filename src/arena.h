#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>

typedef struct {
    void *data;
    size_t size;
    size_t count;
} arena_t;

arena_t *arena_new(size_t size);
void *arena_alloc(arena_t *arena, size_t size);
void arena_free(arena_t *arena);

#endif // ARENA_H
