#ifndef TEXT_H
#define TEXT_H

#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "arena.h"

#define FONT_CHAR_START ' '
#define FONT_CHAR_END '~'
#define FONT_CHAR_COUNT (FONT_CHAR_END - FONT_CHAR_START + 1)

typedef struct {
    arena_t *glyphs;
    SDL_Texture *glyphs_ptr[FONT_CHAR_COUNT];
    int chars_width[FONT_CHAR_COUNT];
    int chars_height[FONT_CHAR_COUNT];
    int min_char_height;
} font_atlas;

font_atlas *create_font_atlas(SDL_Renderer *renderer,
                              const char *font_path,
                              int ptsize, SDL_Color fg);
void render_text(SDL_Renderer *renderer, font_atlas *atlas,
                 const char *str, int x, int y);

#endif // TEXT_H
