#include "Cat.hpp"
#include <iostream>
#include <cmath>

Cat::Cat(SDL_Renderer* Ren, const int& width, const int& height) {
	CatTex = Texture::LoadTexture("assets/textures/cat.png", Ren);
	DeadCatTex = Texture::LoadTexture("assets/textures/deadcat.png", Ren);
	Renderer = Ren;
	DestRect.w = height / 15;
	DestRect.h = DestRect.w;

	CenterY = height / 2;
	DestRect.x = width / 2 - (DestRect.w / 2);
	DestRect.y = CenterY - (DestRect.h / 2);
	CatY = DestRect.y;

	IdleOffset = 0;
	Velocity = 0;
	Gravity = DestRect.h * 45;
	KeyFrame = 0;

	State = Ready;
}

Cat::~Cat() {
	SDL_DestroyTexture(CatTex);
}

void Cat::ResetCat() {
	CatY = CenterY - (DestRect.h / 2);
	DestRect.y = CatY;
	Velocity = 0;
	KeyFrame = 0;
	State = Ready;
}

void Cat::Jump() {
	if(State == Ready) {
		State = Alive;
	}

	Velocity = DestRect.h * -12.5;
}

void Cat::ApplyGravity(const int& DeltaTime) {
	Velocity += (Gravity * static_cast<float> (DeltaTime)) / static_cast<float>(1000);
	CatY += static_cast<float>(Velocity * DeltaTime) / static_cast <float>(1000);
	DestRect.y = std::round(CatY);
}

bool Cat::Update(const int& DeltaTime, Map* TheMap,bool& DeathDelay) {	
	switch (State) {
		case Ready:
			Idle(DeltaTime);
			break;

		case Alive:
			if (TheMap->CheckCollision(DestRect.x, DestRect.y, DestRect.w, DestRect.h)) {
				DeathDelay = true;
				Velocity = DestRect.h * -6;
				State = Dead;
				return true;
			}
			ApplyGravity(DeltaTime);
			break;

		case Dead:
			if (CatY <= CenterY * 2) ApplyGravity(DeltaTime);
			break;
	}
	return false;
}

void Cat::Render() {
	switch (State) {
		case Dead:
			SDL_RenderCopy(Renderer, DeadCatTex, NULL, &DestRect);
			return;
		default:
			SDL_RenderCopy(Renderer, CatTex, NULL, &DestRect);
			break;
	}
}

void Cat::Idle(const int& DeltaTime) {
	float x = static_cast<float>(DeltaTime) / static_cast<float>(1000);
	KeyFrame += x * 5;
	if (KeyFrame > 6.2831) KeyFrame = 0;

	IdleOffset = (std::sin(KeyFrame)) * DestRect.h / 6;
	DestRect.y = std::round(CatY + IdleOffset);
}