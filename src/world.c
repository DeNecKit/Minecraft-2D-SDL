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
    int i = 0;
    for (int dx = 0; dx < WORLD_WIDTH; dx++) {
        int x = dx - WORLD_WIDTH / 2;
#define NOISE(x, seed) noise1d((x / 1500.f) * M_PI + M_PI, 2.f, 0.8f, 5, seed)
        int grass_y = NOISE(x, seed) * GRASS_RANGE + GRASS_MIN;
        int dirt_y = grass_y - (NOISE(x, seed + 5) * DIRT_RANGE + DIRT_MIN);
        for (int y = WORLD_HEIGHT - 1; y >= 0; y--) {
            block_type_t type = BLOCK_NONE;
            if (y == grass_y) type = BLOCK_GRASS;
            else if (y < grass_y) {
                if (y >= dirt_y) type = BLOCK_DIRT;
                else type = BLOCK_STONE;
            }
            (*world)[i++] = (block_t) { type, x, y };
        }
    }
}

void render_world(world_t *world,
                  SDL_Renderer *renderer, player_t *player)
{
    for (int i = 0; i < WORLD_SIZE; i++) {
        if ((*world)[i].type == BLOCK_NONE) continue;
        float x = ((*world)[i].x - player->x) * BLOCK_SIZE;
        x += WINDOW_WIDTH / 2.f;
        float y = ((*world)[i].y - player->y) * -BLOCK_SIZE;
        y += WINDOW_HEIGHT / 2.f;
        if (x + BLOCK_SIZE < 0 || y + BLOCK_SIZE < 0 ||
            x > WINDOW_WIDTH || y > WINDOW_HEIGHT) continue;
        SDL_Rect dst_rect = { x, y, BLOCK_SIZE, BLOCK_SIZE };
        SDL_RenderCopy(renderer, texture_blocks,
            &block_rects[(*world)[i].type],
            &dst_rect);
    }
}

SDL_Texture *texture_blocks = NULL;
