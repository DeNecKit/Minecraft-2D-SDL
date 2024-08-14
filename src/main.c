#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

#define ERROR_EXIT(...) do { \
    fprintf(stderr, "ERROR: " __VA_ARGS__); \
    exit(1); \
} while (0)

#define GAME_VERSION "1.0.0-alpha"
#define WINDOW_TITLE "Minecraft 2D " GAME_VERSION
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_FLAGS SDL_WINDOW_SHOWN

#define BLOCK_SIZE 16
#define WORLD_WIDTH (WINDOW_WIDTH / BLOCK_SIZE) // 160
#define WORLD_HEIGHT (WINDOW_HEIGHT / BLOCK_SIZE) // 90
#define WORLD_SIZE (WORLD_WIDTH * WORLD_HEIGHT)
#define GRASS_MIN 10
#define GRASS_MAX 20
#define DIRT_MIN 3
#define DIRT_MAX 7

typedef enum {
    BLOCK_NONE,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_STONE
} block_type_t;

typedef struct {
    block_type_t type;
    SDL_Rect rect;
} block_t;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool is_fullscreen = false;

float noise(float x, float y, float z, float lacunarity, float gain, int octaves, unsigned char seed)
{
    int i;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float sum = 0.0f;

    for (i = 0; i < octaves; i++) {
        sum += stb_perlin_noise3_internal(x*frequency,y*frequency,z*frequency,0,0,0,(unsigned char)((i+seed)%256))*amplitude;
        frequency *= lacunarity;
        amplitude *= gain;
    }
    return fabs(sum);
}

float noise1d(float x, float lacunarity, float gain, int octaves, unsigned char seed)
{
    return noise(x, 0, 0, lacunarity, gain, octaves, seed);
}

float noise2d(float x, float y, float lacunarity, float gain, int octaves, unsigned char seed)
{
    return noise(x, y, 0, lacunarity, gain, octaves, seed);
}

SDL_Texture* load_texture(const char* path)
{
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load(path);
    
    if (surface == NULL) {
        fprintf(stderr, "Could not load image `%s`: %s",
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

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

    int seed = rand() % 256;
    block_t world[WORLD_SIZE] = { 0 };
    for (int x = 0; x < WORLD_WIDTH; x++) {
        #define NOISE(x, seed) noise1d(x / 256.f, 2.f, 0.8f, 5, seed)
        int grass_y = NOISE(x, seed) * (GRASS_MAX - GRASS_MIN) + GRASS_MIN;
        int dirt_y = NOISE(x, seed + 5) * (DIRT_MAX - DIRT_MIN) + DIRT_MIN + grass_y;
        for (int y = 0; y < WORLD_HEIGHT; y++) {
            int i = x * WORLD_HEIGHT + y;
            if (y >= grass_y) {
                block_type_t type = BLOCK_NONE;
                if (y == grass_y) type = BLOCK_GRASS;
                else if (y <= dirt_y) type = BLOCK_DIRT;
                else type = BLOCK_STONE;
                world[i] = (block_t) {
                    .type = type,
                    .rect = {
                        x*BLOCK_SIZE, y*BLOCK_SIZE,
                        BLOCK_SIZE, BLOCK_SIZE
                    }
                };
            }
        }
    }

    SDL_Event event;
    bool running = true;
    uint64_t cur_time, last_time = SDL_GetTicks64();
    int ticks;

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
                    SDL_SetWindowSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
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
                SDL_RenderCopy(renderer, texture_blocks,
                    &blocks_rects[world[i].type],
                    &world[i].rect);
            }
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
