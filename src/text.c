#include <assert.h>
#include "text.h"
#include "util.h"

static arena_t *cur_arena;

void *text_malloc(size_t size)
{
    return arena_alloc(cur_arena, size);
}

void *text_calloc(size_t nmemb, size_t size)
{
    return arena_alloc(cur_arena, nmemb * size);
}

void *text_realloc(void *mem, size_t size)
{
    (void) mem;
    (void) size;
    ERROR_EXIT("This function is just a placeholder,"
               "should be unreachable\n");
    return NULL;
}

void text_free(void *mem)
{
    (void) mem;
    ERROR_EXIT("This function is just a placeholder,"
               "should be unreachable\n");
}

void create_font_atlas(const char *font_path, int ptsize,
                       SDL_Color fg, arena_t *arena,
                       SDL_Surface **atlas)
{
    assert(arena != NULL);
    assert(atlas != NULL);
    
    TTF_Font *font = TTF_OpenFont(font_path, ptsize);
    if (font == NULL) {
        ERROR_EXIT("Failed to load font: %s\n",
                   TTF_GetError());
    }

    cur_arena = arena;

    SDL_malloc_func sdl_malloc = NULL;
    SDL_calloc_func sdl_calloc = NULL;
    SDL_realloc_func sdl_realloc = NULL;
    SDL_free_func sdl_free = NULL;
    SDL_GetMemoryFunctions(&sdl_malloc, &sdl_calloc,
                           &sdl_realloc, &sdl_free);
    if (SDL_SetMemoryFunctions(
        text_malloc, text_calloc,
        text_realloc, text_free) != 0) {
        ERROR_EXIT("Failed to set custom SDL memory functions: %s\n",
                   SDL_GetError());
    }
    for (char c = FONT_CHAR_START; c <= FONT_CHAR_END; c++) {
        atlas[c - FONT_CHAR_START] =
            TTF_RenderGlyph32_Solid(font, c, fg);
    }
    if (SDL_SetMemoryFunctions(
        sdl_malloc, sdl_calloc,
        sdl_realloc, sdl_free) != 0) {
        ERROR_EXIT("Should be unreachable\n");
    }

    TTF_CloseFont(font);
}

void render_text(SDL_Surface **atlas,
                 const char *str, int x, int y,
                 SDL_Surface *surface_dst)
{
    assert(atlas != NULL);
    assert(atlas[0] != NULL);
    SDL_Rect char_dst = { x, y, 0, 0 };
    for (const char *ptr = str; *ptr != 0; ptr++) {
        const char c = *ptr;
        if (c == '\n') {
            char_dst.x = x;
            char_dst.y += atlas[0]->h;
            continue;
        }
        SDL_Surface *char_surface = atlas[c - FONT_CHAR_START];
        SDL_BlitSurface(char_surface, NULL, surface_dst, &char_dst);
        char_dst.x += char_surface->w;
    }
}
