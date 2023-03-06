#pragma once
#include "SDL_image.h"
#include "SDL.h"

namespace Texture {
	SDL_Texture* LoadTexture(const char* path, SDL_Renderer* Renderer);
}