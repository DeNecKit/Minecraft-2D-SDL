#include "texture.h"
#include <stdio.h>
#include <SDL_image.h>

SDL_Texture* load_texture(const char *path, SDL_Renderer *renderer)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load(path);
    
    if (surface == NULL) {
        fprintf(stderr, "Could not load image `%s`: %s\n",
                        path, IMG_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        fprintf(stderr, "Could not create texture from `%s`: %s",
                        path, SDL_GetError());
        return NULL;
    }

    SDL_FreeSurface(surface);
    return texture;
}
