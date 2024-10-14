#include "world.h"
#include "block.h"
#include "graphics.h"

void generate_world(world_t *world, u64 seed)
{
    int i = 0;
    for (int dy = 0; dy < WORLD_CHUNKS_HEIGHT; dy++) {
        int y = (int)floorf(player.pos.y / CHUNK_WIDTH) + dy;
        y -= WORLD_CHUNKS_HEIGHT / 2;
        if (y < 0 || y >= WORLD_HEIGHT / CHUNK_HEIGHT) {
            i += WORLD_CHUNKS_WIDTH;
            continue;
        }
        for (int dx = 0; dx < WORLD_CHUNKS_WIDTH; dx++) {
            int x = (int)floorf(player.pos.x / CHUNK_WIDTH) + dx;
            x -= WORLD_CHUNKS_WIDTH / 2;
            world->chunks[i].pos = (vec2){ x, y };
            chunk_generate(&world->chunks[i++], seed);
        }
    }
}

void render_world(world_t *world, SDL_Renderer *renderer)
{
    for (int i = 0; i < WORLD_CHUNKS_NUM; i++)
        chunk_render(&world->chunks[i], renderer);
    if (chunk_borders) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                float px = player.pos.x;
                float py = player.pos.y;
                int x0 = dx * CHUNK_WIDTH;
                x0 += floorf(px / CHUNK_WIDTH) * CHUNK_WIDTH;
                x0 = (x0 - px) * BLOCK_SIZE + WINDOW_WIDTH / 2;
                int y0 = dy * CHUNK_HEIGHT;
                y0 += floorf(py / CHUNK_HEIGHT) * CHUNK_HEIGHT;
                y0 = -((y0 - py) * BLOCK_SIZE + WINDOW_HEIGHT / 2);
                int w = CHUNK_WIDTH * BLOCK_SIZE;
                int h = CHUNK_HEIGHT * BLOCK_SIZE;
                G_RenderDrawRect(renderer, COLOR(0xff0000ff),
                    x0, y0, w, h, 2);
            }
        }
    }
}

SDL_Texture *texture_blocks = NULL;
