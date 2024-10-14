#include "chunk.h"
#include "world.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"


bool chunk_borders = false;

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

void chunk_generate(chunk_t *chunk, u64 seed)
{
    assert(chunk != NULL);
    int i = 0;
    int x = chunk->pos.x * CHUNK_WIDTH;
    int y = chunk->pos.y * CHUNK_HEIGHT;
    for (int dx = 0; dx < CHUNK_WIDTH; dx++, x++) {
#define NOISE(x, seed) noise1d((x / 500.f) * M_PI + M_PI, 5, seed)
        int grass_y = NOISE(x, seed) * GRASS_RANGE + GRASS_MIN;
        int dirt_y = grass_y - (NOISE(x, seed + 5) * DIRT_RANGE + DIRT_MIN);
        for (int dy = 0; dy < CHUNK_HEIGHT; dy++, y++) {
            block_type_t type = BLOCK_NONE;
            if (y <= grass_y) {
                if (y == grass_y) type = BLOCK_GRASS;
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

            }
            chunk->blocks[i++] = (block_t) { type, (vec2b){ dx, dy } };
        }
        y -= CHUNK_HEIGHT;
    }
}

void chunk_render(chunk_t *chunk, SDL_Renderer *renderer)
{
    if (chunk->is_updated || chunk->texture == NULL) {
        if (chunk->texture == NULL) {
            chunk->texture = SDL_CreateTexture(renderer,
                SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
                BLOCK_SIZE * CHUNK_WIDTH, BLOCK_SIZE * CHUNK_HEIGHT);
            SDL_SetTextureBlendMode(chunk->texture, SDL_BLENDMODE_BLEND);
        }
        SDL_SetRenderTarget(renderer, chunk->texture);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        for (int i = 0; i < CHUNK_SIZE; i++) {
            if (chunk->blocks[i].type == BLOCK_NONE) continue;
            float x = chunk->blocks[i].rel_pos.x * BLOCK_SIZE;
            float y = (CHUNK_HEIGHT - chunk->blocks[i].rel_pos.y - 1) * BLOCK_SIZE;
            SDL_Rect dst_rect = { x, y, BLOCK_SIZE, BLOCK_SIZE };
            SDL_RenderCopy(renderer, texture_blocks,
                &block_rects[chunk->blocks[i].type],
                &dst_rect);
        }
        SDL_SetRenderTarget(renderer, NULL);
        chunk->is_updated = false;
    } else {
        float x = (chunk->pos.x * CHUNK_WIDTH - player.pos.x);
        x = (x * BLOCK_SIZE) + WINDOW_WIDTH / 2.f;
        float y = (chunk->pos.y * CHUNK_HEIGHT - player.pos.y);
        y = (y * BLOCK_SIZE) + WINDOW_HEIGHT / 2.f;
        y *= -1;
        int w = BLOCK_SIZE * CHUNK_WIDTH;
        int h = BLOCK_SIZE * CHUNK_HEIGHT;
        SDL_Rect dst = { x, y, w, h };
        SDL_RenderCopy(renderer, chunk->texture, NULL, &dst);
    }
}
