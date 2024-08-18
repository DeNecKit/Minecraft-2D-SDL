#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "util.h"
#include "arena.h"
#include "window.h"
#include "texture.h"
#include "text.h"
#include "world.h"
#include "block.h"
#include "player.h"

// TODO: minimap

#define FONT_F3_SIZE 240000

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool is_fullscreen = false;

arena_t *font_f3_arena = NULL;
SDL_Surface *font_f3_atlas[FONT_CHAR_COUNT] = { NULL };
SDL_Texture *f3_texture = NULL;
SDL_Surface *f3_surface = NULL;
int fps = 0;

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

    if (TTF_Init() == -1) {
        ERROR_EXIT("Failed to initialize SDL_ttf: %s\n",
                   TTF_GetError());
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

    texture_blocks = load_texture(renderer,
        "assets/textures/blocks.png");
    if (texture_blocks == NULL) {
        ERROR_EXIT("Failed to load `blocks` texture\n");
    }

    font_f3_arena = arena_new(FONT_F3_SIZE);
    create_font_atlas("assets/font/Minecraft.otf", 23,
        (SDL_Color) { 0xff, 0xff, 0xff, 0xff },
        font_f3_arena, font_f3_atlas);

    f3_texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetTextureBlendMode(f3_texture, SDL_BLENDMODE_BLEND);

    printf("%lld/%lld\n", font_f3_arena->count, font_f3_arena->size);

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
    char f3_str[64] = { 0 };

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
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
            fps = (float)ticks / (cur_time - last_fps_time) * 1000;
            last_fps_time = cur_time;
            ticks = 0;
        }

        player_update(&player, dt);

        SDL_RenderClear(renderer);
        
        render_world(&world, renderer, &player);

        SDL_LockTextureToSurface(f3_texture, NULL, &f3_surface);
        SDL_FillRect(f3_surface, NULL, 0);
        sprintf(f3_str, "%d FPS\nX: %.2f, Y: %.2f",
                        fps, player.x, player.y);
        render_text(font_f3_atlas, f3_str, 15, 15, f3_surface);
        SDL_UnlockTexture(f3_texture);
        SDL_RenderCopy(renderer, f3_texture, NULL, NULL);
        
        SDL_RenderPresent(renderer);
    }


    SDL_DestroyTexture(texture_blocks);
    SDL_DestroyTexture(f3_texture);
    arena_free(font_f3_arena);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
