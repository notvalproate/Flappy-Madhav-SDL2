#include "Cat.hpp"
#include <iostream>

Cat::Cat(const char* texturepath, SDL_Renderer* Ren, const int& width, const int& height) {
	CatTex = Texture::LoadTexture(texturepath, Ren);
	Renderer = Ren;
	if (width < height) {
		DestRect.w = width / 15;
		DestRect.h = DestRect.w;
	}
	else {
		DestRect.w = height / 15;
		DestRect.h = DestRect.w;
	}
	x = width / 2;
	y = height / 2;
	DestRect.x = x - (DestRect.w / 2);
	DestRect.y = y - (DestRect.h / 2);
	Velocity = 0;
	Gravity = DestRect.h * 45;
	Dead = true;
	Start = true;
}

Cat::~Cat() {
	SDL_DestroyTexture(CatTex);
}

void Cat::Jump(Map* TheMap, Audio* JumpSound) {
	if (Start) {
		Start = false;
		Dead = false;
	}

	if (Dead) {
		DestRect.y = y - (DestRect.h / 2);
		Velocity = 0;
		TheMap->ResetMap();
		Start = true;
		return;
	}

	Velocity = DestRect.h * -11;
	JumpSound->PlaySound();
}

void Cat::Update(const int& DeltaTime, const int& height, Map* TheMap, Audio* DeathSound) {
	if (Dead) {
		return;
	}
	if (TheMap->CheckCollision(DestRect.x, DestRect.y, DestRect.w, DestRect.h)) {
		DeathSound->PlaySound();
		Dead = true;
	}

	Velocity += Gravity * (DeltaTime / 1000.0);
	DestRect.y += Velocity * (DeltaTime / 1000.0);
}

void Cat::Render() {
	SDL_RenderCopy(Renderer, CatTex, NULL, &DestRect);
}