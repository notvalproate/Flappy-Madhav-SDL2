#include "Floor.hpp"
#include "Texture.hpp"
#include <cmath>

Floor::Floor(const char* texturepath, SDL_Renderer* Ren, const int& width, const int& height) {
	Renderer = Ren;

	FloorTex = Texture::LoadTexture(texturepath, Ren);

	SDL_Point size;
	SDL_QueryTexture(FloorTex, NULL, NULL, &size.x, &size.y);
	SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = size.x;
	SrcRect.h = 10;

	FirstHalf.y = (height * 9) / 10;
	FirstHalf.w = width;
	FirstHalf.h = height / 10;

	SecondHalf.y = FirstHalf.y;
	SecondHalf.w = FirstHalf.w;
	SecondHalf.h = FirstHalf.h;

	ResetFloor();

	Velocity = width / 4.5;
}

Floor::~Floor() { 
	SDL_DestroyTexture(FloorTex);
}

void Floor::ResetFloor() {
	Distance = 0;
	FirstHalf.x = 0;
	SecondHalf.x = FirstHalf.w;
}

void Floor::Update(const int& DeltaTime) {
	Distance -= static_cast<float> (Velocity * DeltaTime) / static_cast <float> (1000);
	FirstHalf.x = std::round(Distance);
	SecondHalf.x = std::round(Distance) + FirstHalf.w;

	if (SecondHalf.x <= 0) {
		ResetFloor();
	}
}

void Floor::Render() {
	SDL_RenderCopy(Renderer, FloorTex, &SrcRect, &FirstHalf);
	SDL_RenderCopy(Renderer, FloorTex, &SrcRect, &SecondHalf);
}