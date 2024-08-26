#include "text.h"
#include "util.h"

void create_font_atlas(SDL_Renderer *renderer,
                       font_atlas *atlas,
                       const char *font_path,
                       int ptsize, SDL_Color fg)
{
    assert(atlas != NULL);

    TTF_Font *font = TTF_OpenFont(font_path, ptsize);
    if (font == NULL) {
        ERROR_EXIT("Failed to load font: %s\n",
                   TTF_GetError());
    }

    SDL_Texture *textures_tmp[FONT_CHAR_COUNT] = { NULL };
    atlas->min_char_height = INT_MAX;
    int max_char_height = -1;

    SDL_Rect cur_glyph = { 0 };
    for (char c = FONT_CHAR_START; c <= FONT_CHAR_END; c++) {
        SDL_Surface *surface = TTF_RenderGlyph32_Solid(font, c, fg);
        if (surface == NULL) {
            ERROR_EXIT("Failed to create a glyph surface: %s\n",
                       TTF_GetError());
        }
        const int i = c - FONT_CHAR_START;
        textures_tmp[i] =
            SDL_CreateTextureFromSurface(renderer, surface);
        if (textures_tmp[i] == NULL) {
            ERROR_EXIT("Failed to create a glyph texture: %s\n",
                       SDL_GetError());
        }
        cur_glyph.w = surface->w;
        cur_glyph.h = surface->h;
        atlas->glyphs[i] = cur_glyph;
        if (cur_glyph.h < atlas->min_char_height) {
            atlas->min_char_height = cur_glyph.h;
        }
        if (cur_glyph.h > max_char_height) {
            max_char_height = cur_glyph.h;
        }
        cur_glyph.x += cur_glyph.w;
        SDL_FreeSurface(surface);
    }

    atlas->texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET, cur_glyph.x, max_char_height);
    SDL_SetTextureBlendMode(atlas->texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, atlas->texture);
    for (char c = FONT_CHAR_START; c <= FONT_CHAR_END; c++) {
        const int i = c - FONT_CHAR_START;
        SDL_RenderCopy(renderer, textures_tmp[i], NULL, &atlas->glyphs[i]);
        SDL_DestroyTexture(textures_tmp[i]);
    }
    SDL_SetRenderTarget(renderer, NULL);
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
        char_dst.w = atlas->glyphs[i].w;
        char_dst.h = atlas->glyphs[i].h;
        SDL_RenderCopy(renderer, atlas->texture,
            &atlas->glyphs[i], &char_dst);
        char_dst.x += atlas->glyphs[i].w;
        if (atlas->glyphs[i].h > max_height) {
            max_height = atlas->glyphs[i].h;
        }
    }
}

void free_font_atlas(font_atlas *atlas)
{
    SDL_DestroyTexture(atlas->texture);
}
