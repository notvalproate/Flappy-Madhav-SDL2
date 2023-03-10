#include "Texture.hpp"

SDL_Texture* Texture::LoadTexture(const char* path, SDL_Renderer* Renderer) {
	//Create a temp surface to load img data into
	SDL_Surface* TempSurface = IMG_Load(path);

	//Create a texture using that surface
	SDL_Texture* Tex = SDL_CreateTextureFromSurface(Renderer, TempSurface);

	//Free the surface
	SDL_FreeSurface(TempSurface);

	return Tex;
}