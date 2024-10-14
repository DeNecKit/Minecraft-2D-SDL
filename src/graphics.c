#include "graphics.h"
#include "util.h"

int G_RenderDrawRect(SDL_Renderer *renderer, SDL_Color clr,
                     int x, int y, int w, int h, int boldness)
{
    assert(renderer != NULL);
    assert(boldness > 0);
    assert(w > 0);
    assert(h > 0);
    SDL_Color tmp_clr = { 0 };
    SDL_GetRenderDrawColor(renderer,
        &tmp_clr.r, &tmp_clr.g, &tmp_clr.b, &tmp_clr.a);
    SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, clr.a);
    SDL_Rect dst = { x, y, w, h };
    int res = SDL_RenderDrawRect(renderer, &dst);
    int offset = 1;
    while (--boldness > 0) {
        dst = (SDL_Rect){ x + offset, y + offset, w, h };
        res = res || SDL_RenderDrawRect(renderer, &dst);
        dst = (SDL_Rect){ x - offset, y - offset, w, h };
        res = res || SDL_RenderDrawRect(renderer, &dst);
        offset++;
    }
    SDL_SetRenderDrawColor(renderer,
        tmp_clr.r, tmp_clr.g, tmp_clr.b, tmp_clr.a);
    return res;
}
