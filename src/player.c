#include "player.h"
#include "util.h"
#include <SDL.h>

#define PLAYER_VEL 25.f

void player_update(player_t *player, float dt)
{
    const u8 *kb_state = SDL_GetKeyboardState(NULL);
    if (kb_state[SDL_SCANCODE_A]) player->x -= PLAYER_VEL * dt;
    else if (kb_state[SDL_SCANCODE_D]) player->x += PLAYER_VEL * dt;
    if (kb_state[SDL_SCANCODE_W]) player->y += PLAYER_VEL * dt;
    else if (kb_state[SDL_SCANCODE_S]) player->y -= PLAYER_VEL * dt;
}
