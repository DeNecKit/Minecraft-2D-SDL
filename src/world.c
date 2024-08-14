#include "world.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include <math.h>

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

void generate_world(world_t *world, unsigned char seed)
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
