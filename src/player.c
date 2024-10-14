#include <SDL.h>

#include "player.h"
#include "util.h"
#include "world.h"

#define PLAYER_VEL 25.f

player_t player = { .pos = { .x = 0, .y = GRASS_MIN } };

void player_update(float dt)
{
    const u8 *kb_state = SDL_GetKeyboardState(NULL);
    if (kb_state[SDL_SCANCODE_A]) player.pos.x -= PLAYER_VEL * dt;
    if (kb_state[SDL_SCANCODE_D]) player.pos.x += PLAYER_VEL * dt;
    if (kb_state[SDL_SCANCODE_W]) player.pos.y += PLAYER_VEL * dt;
    if (kb_state[SDL_SCANCODE_S]) player.pos.y -= PLAYER_VEL * dt;
}
