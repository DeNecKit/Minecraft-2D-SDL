#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>

int G_RenderDrawRect(SDL_Renderer *renderer, SDL_Color clr,
                     int x, int y, int w, int h, int boldness);

#endif // GRAPHICS_H
