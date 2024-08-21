#include "text.h"
#include "util.h"

void create_font_atlas(SDL_Renderer *renderer,
                       font_atlas *atlas,
                       const char *font_path,
                       int ptsize, SDL_Color fg)
{
    TTF_Font *font = TTF_OpenFont(font_path, ptsize);
    if (font == NULL) {
        ERROR_EXIT("Failed to load font: %s\n",
                   TTF_GetError());
    }

    atlas->min_char_height = INT_MAX;
    for (char c = FONT_CHAR_START; c <= FONT_CHAR_END; c++) {
        SDL_Surface *surface = TTF_RenderGlyph32_Solid(font, c, fg);
        if (surface == NULL) {
            ERROR_EXIT("Failed to create a glyph surface: %s\n",
                       TTF_GetError());
        }
        const int i = c - FONT_CHAR_START;
        atlas->glyphs_ptr[i] =
            SDL_CreateTextureFromSurface(renderer, surface);
        if (atlas->glyphs_ptr[i] == NULL) {
            ERROR_EXIT("Failed to create a glyph texture: %s\n",
                       SDL_GetError());
        }
        atlas->chars_width[i] = surface->w;
        atlas->chars_height[i] = surface->h;
        if (atlas->chars_height[i] < atlas->min_char_height) {
            atlas->min_char_height = atlas->chars_height[i];
        }
        SDL_FreeSurface(surface);
    }

    TTF_CloseFont(font);
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
