#include "Texture.hpp"

SDL_Texture* Texture::LoadTexture(const char* path, SDL_Renderer* Renderer) {
	SDL_Surface* TempSurface = IMG_Load(path);
	SDL_Texture* Tex = SDL_CreateTextureFromSurface(Renderer, TempSurface);
	SDL_FreeSurface(TempSurface);

	return Tex;
}