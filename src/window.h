#ifndef WINDOW_H
#define WINDOW_H

#include "SDL.h"

#define GAME_VERSION "v1.1.1-alpha"
#define WINDOW_TITLE "Minecraft 2D " GAME_VERSION
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_FLAGS SDL_WINDOW_SHOWN
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

#endif // WINDOW_H
