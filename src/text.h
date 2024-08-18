#ifndef TEXT_H
#define TEXT_H

#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "arena.h"

#define FONT_CHAR_START ' '
#define FONT_CHAR_END '~'
#define FONT_CHAR_COUNT (FONT_CHAR_END - FONT_CHAR_START + 1)

void create_font_atlas(const char *font_path, int ptsize,
                       SDL_Color fg, arena_t *arena,
                       SDL_Surface **atlas);
void render_text(SDL_Surface **atlas,
                 const char *str, int x, int y,
                 SDL_Surface *surface_dst);

#endif // TEXT_H
