#include "world.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include <math.h>
#include "block.h"

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

float noise1d(float x, float lacunarity, float gain, int octaves, u64 seed)
{
    return noise(x, 0, seed, lacunarity, gain, octaves, (unsigned char)seed);
}

float noise2d(float x, float y, float lacunarity, float gain, int octaves, u64 seed)
{
    return noise(x, y, seed, lacunarity, gain, octaves, (unsigned char)seed);
}

void generate_world(world_t *world, u64 seed)
{
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
                (*world)[i] = (block_t) { type, x, y };
            }
        }
    }
}

SDL_Texture *texture_blocks = NULL;

void render_world(world_t *world, SDL_Renderer *renderer)
{
    for (int i = 0; i < WORLD_SIZE; i++) {
        if ((*world)[i].type != BLOCK_NONE) {
            SDL_Rect dst_rect = {
                .x = (*world)[i].x * BLOCK_SIZE,
                .y = (*world)[i].y * BLOCK_SIZE,
                BLOCK_SIZE, BLOCK_SIZE
            };
            SDL_RenderCopy(renderer, texture_blocks,
                &block_rects[(*world)[i].type],
                &dst_rect);
        }
    }
}
