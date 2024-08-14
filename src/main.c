#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

#include "util.h"
#include "window.h"
#include "world.h"
#include "block.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool is_fullscreen = false;

SDL_Texture* load_texture(const char* path)
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

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    srand(time(0));

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        ERROR_EXIT("Failed to initialize SDL: %s\n",
                   SDL_GetError());
    }

    const int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        ERROR_EXIT("Failed to initialize SDL_image: %s\n",
                   IMG_GetError());
    }

    window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        WINDOW_FLAGS
    );
    if (window == NULL) {
        ERROR_EXIT("Failed to create window: %s\n",
                   SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
    if (renderer == NULL) {
        ERROR_EXIT("Failed to create renderer: %s\n",
                   SDL_GetError());
    }
    SDL_SetRenderDrawColor(renderer, 0xb0, 0xd6, 0xf5, 0xff);

    SDL_Texture *texture_blocks = load_texture(
        "assets/textures/blocks.png");
    if (texture_blocks == NULL) {
        ERROR_EXIT("Failed to load `blocks` texture\n");
    }
    
    SDL_Rect blocks_rects[] = {
        { 0 },
        { 0,  0, 16, 16 },
        { 16, 0, 16, 16 },
        { 32, 0, 16, 16 }
    };

    world_t world = { 0 };
    generate_world(&world, rand());

    SDL_Event event;
    bool running = true;
    u64 cur_time, last_time = SDL_GetTicks64();
    int ticks = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN &&
                       event.key.keysym.sym == SDLK_F11) {
                is_fullscreen = !is_fullscreen;
                if (is_fullscreen) {
                    SDL_DisplayMode dm;
                    SDL_GetCurrentDisplayMode(0, &dm);
                    SDL_SetWindowSize(window, dm.w, dm.h);
                    SDL_SetWindowFullscreen(window,
                        SDL_WINDOW_FULLSCREEN_DESKTOP);
                } else {
                    SDL_SetWindowFullscreen(window, 0);
                    SDL_SetWindowSize(window,
                        WINDOW_WIDTH, WINDOW_HEIGHT);
                }
            }
        }

        ticks++;
        cur_time = SDL_GetTicks64();
        if (cur_time - last_time >= 1000) {
            printf("FPS: %f\n",
                   (float)ticks / (cur_time - last_time) * 1000);
            last_time = cur_time;
            ticks = 0;
        }

        SDL_RenderClear(renderer);
        for (int i = 0; i < WORLD_SIZE; i++) {
            if (world[i].type != BLOCK_NONE) {
                SDL_Rect dst_rect = {
                    .x = world[i].x * BLOCK_SIZE,
                    .y = world[i].y * BLOCK_SIZE,
                    BLOCK_SIZE, BLOCK_SIZE
                };
                SDL_RenderCopy(renderer, texture_blocks,
                    &blocks_rects[world[i].type],
                    &dst_rect);
            }
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
