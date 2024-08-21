#include "world.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include <math.h>
#include "block.h"

float noise3d(float x, float y, float z, int octaves)
{
    float val = stb_perlin_fbm_noise3(x, y, z, 2.f, 0.5f, octaves);
    val = (val + 1.15f) / 2.275f;
    return val;
}

float noise1d(float x, int octaves, u64 seed)
{
    return noise3d(x, seed, 0, octaves);
}

float noise2d(float x, float y, int octaves, u64 seed)
{
    return noise3d(x, y, seed, octaves);
}

void generate_world(world_t world, u64 seed)
{
    assert(world != NULL);
    int i = 0;
    for (int dx = 0; dx < WORLD_WIDTH; dx++) {
        int x = dx - WORLD_WIDTH / 2;
#define NOISE(x, seed) noise1d((x / 500.f) * M_PI + M_PI, 5, seed)
        int grass_y = NOISE(x, seed) * GRASS_RANGE + GRASS_MIN;
        int dirt_y = grass_y - (NOISE(x, seed + 5) * DIRT_RANGE + DIRT_MIN);
        for (int y = WORLD_HEIGHT - 1; y >= 0; y--) {
            block_type_t type = BLOCK_NONE;
            if (y > grass_y) continue;
            else if (y == grass_y) type = BLOCK_GRASS;
            else if (y >= dirt_y) type = BLOCK_DIRT;
            else type = BLOCK_STONE;

            const float k = 50.f * M_PI;
            float cave_type = noise2d(x / k, y / k, 6, seed * M_PI);
            float y_coef = (float)y / ((GRASS_MAX + GRASS_MIN) / 2.f) - 0.5f;
            y_coef *= y_coef;
            y_coef = (0.25f - y_coef) * 4.f;
            cave_type = (cave_type - 0.5f) * y_coef + 0.5f;
            float ch = 0.f;
            const float cave_range = 0.1f;
            if (cave_type > 0.5f + cave_range / 2.f) {
                const float k1 = k / 3.f;
                ch = noise2d(x / k1, y / k1, 6, seed * (M_PI + 1));
                if (ch < 0.35f || ch > 0.65f) type = BLOCK_NONE;
            } else if (cave_type < 0.5f - cave_range / 2.f) {
                const float k2 = k / 2.f;
                ch = noise2d(x / k2, y / k2, 6, seed * (M_PI + 2));
                if (ch > 0.5f && ch < 0.55f) type = BLOCK_NONE;
            }

            world[i++] = (block_t) { type, x, y };
        }
    }
}

void render_world(const world_t world,
                  SDL_Renderer *renderer, player_t *player)
{
    for (int i = 0; i < WORLD_SIZE; i++) {
        if (world[i].type == BLOCK_NONE) continue;
        float x = (world[i].x - player->x) * BLOCK_SIZE;
        x += WINDOW_WIDTH / 2.f;
        float y = (world[i].y - player->y) * -BLOCK_SIZE;
        y += WINDOW_HEIGHT / 2.f;
        if (x + BLOCK_SIZE < 0 || y + BLOCK_SIZE < 0 ||
            x > WINDOW_WIDTH || y > WINDOW_HEIGHT) continue;
        SDL_Rect dst_rect = { x, y, BLOCK_SIZE, BLOCK_SIZE };
        SDL_RenderCopy(renderer, texture_blocks,
            &block_rects[world[i].type],
            &dst_rect);
    }
}

SDL_Texture *texture_blocks = NULL;
