#include "text.h"
#include "util.h"

static arena_t *cur_arena = NULL;

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
    ERROR_EXIT("(realloc) This function is just a placeholder, "
               "should be unreachable\n");
    return NULL;
}

void text_free(void *mem)
{
    (void) mem;
}

font_atlas *create_font_atlas(SDL_Renderer *renderer,
                              const char *font_path,
                              int ptsize, SDL_Color fg)
{
    TTF_Font *font = TTF_OpenFont(font_path, ptsize);
    if (font == NULL) {
        ERROR_EXIT("Failed to load font: %s\n",
                   TTF_GetError());
    }

    const int mem_size = FONT_CHAR_COUNT * ptsize * ptsize * 1.5f * 4;
    font_atlas *atlas = malloc(
        sizeof(font_atlas) + sizeof(arena_t) + mem_size);
    atlas->glyphs = (arena_t*)((char*)atlas + sizeof(font_atlas));
    atlas->glyphs->size = mem_size;
    atlas->glyphs->count = 0;
    atlas->glyphs->data = (char*)atlas + sizeof(font_atlas) + sizeof(arena_t);

    arena_t *tmp_glyphs = arena_new(mem_size);
    SDL_Surface *tmp_glyphs_ptr[FONT_CHAR_COUNT] = { NULL };

    SDL_malloc_func sdl_malloc = NULL;
    SDL_calloc_func sdl_calloc = NULL;
    SDL_realloc_func sdl_realloc = NULL;
    SDL_free_func sdl_free = NULL;
    SDL_GetMemoryFunctions(&sdl_malloc, &sdl_calloc, &sdl_realloc, &sdl_free);
    if (SDL_SetMemoryFunctions(text_malloc, text_calloc,
                               text_realloc, text_free) != 0) {
        ERROR_EXIT("Failed to set custom SDL memory functions: %s\n",
                   SDL_GetError());
    }

    atlas->min_char_height = INT_MAX;
    for (char c = FONT_CHAR_START; c <= FONT_CHAR_END; c++) {
        const int i = c - FONT_CHAR_START;
        
        cur_arena = tmp_glyphs;
        tmp_glyphs_ptr[i] = TTF_RenderGlyph32_Solid(font, c, fg);
        if (tmp_glyphs_ptr[i] == NULL) {
            ERROR_EXIT("Failed to create a glyph surface: %s\n",
                       TTF_GetError());
        }
        
        cur_arena = atlas->glyphs;
        atlas->glyphs_ptr[i] =
            SDL_CreateTextureFromSurface(renderer, tmp_glyphs_ptr[i]);
        if (atlas->glyphs_ptr[i] == NULL) {
            ERROR_EXIT("Failed to create a glyph texture: %s\n",
                       SDL_GetError());
        }
        atlas->chars_width[i] = tmp_glyphs_ptr[i]->w;
        atlas->chars_height[i] = tmp_glyphs_ptr[i]->h;
        if (atlas->chars_height[i] < atlas->min_char_height) {
            atlas->min_char_height = atlas->chars_height[i];
        }
    }

    cur_arena = NULL;
    if (SDL_SetMemoryFunctions(sdl_malloc, sdl_calloc,
                               sdl_realloc, sdl_free) != 0) {
        ERROR_EXIT("Should be unreachable\n");
    }
    TTF_CloseFont(font);
    free(tmp_glyphs);

    return atlas;
}

void render_text(SDL_Renderer *renderer, font_atlas *atlas,
                 const char *str, int x, int y)
{
    SDL_Rect char_dst = { x, y, 0, 0 };
    int max_height = atlas->min_char_height;
    for (const char *ptr = str; *ptr != 0; ptr++) {
        const char c = *ptr;
        if (c == '\n') {
            char_dst.x = x;
            char_dst.y += max_height;
            continue;
        }
        const int i = c - FONT_CHAR_START;
        SDL_Texture *texture = atlas->glyphs_ptr[i];
        char_dst.w = atlas->chars_width[i];
        char_dst.h = atlas->chars_height[i];
        SDL_RenderCopy(renderer, texture, NULL, &char_dst);
        char_dst.x += atlas->chars_width[i];
        if (atlas->chars_height[i] > max_height) {
            max_height = atlas->chars_height[i];
        }
    }
}
