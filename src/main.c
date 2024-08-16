#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#include "util.h"
#include "window.h"
#include "texture.h"
#include "world.h"
#include "block.h"
#include "player.h"

// TODO: minimap

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool is_fullscreen = false;

world_t world = { 0 };
player_t player = { .x = 0, .y = GRASS_MIN };

void init()
{
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

    texture_blocks = load_texture(
        "assets/textures/blocks.png", renderer);
    if (texture_blocks == NULL) {
        ERROR_EXIT("Failed to load `blocks` texture\n");
    }

    block_init();
    
    generate_world(&world, rand());
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    init();

    SDL_Event event;
    bool running = true;
    u64 last_fps_time = SDL_GetTicks64();
    u64 cur_time, last_time = last_fps_time;
    float dt = 0.f;
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
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                       event.button.button == SDL_BUTTON_LEFT) {
                memset(&world, 0, sizeof(world_t));
                generate_world(&world, rand());
            }
        }

        ticks++;
        cur_time = SDL_GetTicks64();
        dt = (cur_time - last_time) / 1000.f;
        last_time = cur_time;
        if (cur_time - last_fps_time >= 1000) {
            printf("FPS: %f\tx: %f\ty: %f\n",
                   (float)ticks / (cur_time - last_fps_time) * 1000,
                   player.x, player.y);
            last_fps_time = cur_time;
            ticks = 0;
        }

        player_update(&player, dt);

        SDL_RenderClear(renderer);
        render_world(&world, renderer, &player);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
