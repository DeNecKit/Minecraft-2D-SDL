#include "arena.h"
#include "util.h"

arena_t *arena_new(size_t size)
{
    void *mem = malloc(sizeof(arena_t) + size);
    arena_t *arena = mem;
    arena->data = (char*)mem + sizeof(arena_t);
    arena->size = size;
    arena->count = 0;
    return arena;
}

void *arena_alloc(arena_t *arena, size_t size)
{
    if (arena->count + size > arena->size) {
        ERROR_EXIT("Arena memory overflow\n");
    }

    void *ptr = (char*)arena->data + arena->count;
    arena->count += size;
    return ptr;
}

void arena_free(arena_t *arena)
{
    free(arena);
}
