#include "Cat.hpp"
#include <iostream>
#include <cmath>

Cat::Cat(SDL_Renderer* Ren, const int& width, const int& height) {
	CatTex = Texture::LoadTexture("assets/textures/cat.png", Ren);
	DeadCatTex = Texture::LoadTexture("assets/textures/deadcat.png", Ren);
	Renderer = Ren;
	DestRect.w = height / 15;
	DestRect.h = DestRect.w;
	x = width / 2;
	y = height / 2;
	DestRect.x = x - (DestRect.w / 2);
	DestRect.y = y - (DestRect.h / 2);
	caty = DestRect.y;
	IdleLoc = 0;
	Velocity = 0;
	Gravity = DestRect.h * 45;
	KeyFrame = 0;
	State = Ready;
}

Cat::~Cat() {
	SDL_DestroyTexture(CatTex);
}

void Cat::ResetCat() {
	DestRect.y = y - (DestRect.h / 2);
	caty = DestRect.y;
	Velocity = 0;
	State = Ready;
}

void Cat::Jump() {
	if(State == Ready) {
		State = Alive;
	}

	Velocity = DestRect.h * -13;
}

void Cat::ApplyGravity(const int& DeltaTime) {
	Velocity += (Gravity * static_cast<float> (DeltaTime)) / static_cast<float>(1000);
	caty += static_cast<float>(Velocity * DeltaTime) / static_cast <float>(1000);
	DestRect.y = std::round(caty);
}

bool Cat::Update(const int& DeltaTime, Map* TheMap,bool& DeathDelay) {	
	switch (State) {
		case Ready:
			Idle(DeltaTime);
			return false;

		case Alive:
			if (TheMap->CheckCollision(DestRect.x, DestRect.y, DestRect.w, DestRect.h)) {
				DeathDelay = true;
				Velocity = DestRect.h * -6;
				State = Dead;
				return true;
			}
			ApplyGravity(DeltaTime);
			return false;

		case Dead:
			if (caty <= y * 2) ApplyGravity(DeltaTime);
			return false;
	}
}

void Cat::Render() {
	switch (State) {
		case Dead:
			SDL_RenderCopy(Renderer, DeadCatTex, NULL, &DestRect);
			break;
		default:
			SDL_RenderCopy(Renderer, CatTex, NULL, &DestRect);
			break;
	}
}

void Cat::Idle(const int& DeltaTime) {
	float x = static_cast<float>(DeltaTime) / static_cast<float>(1000);
	KeyFrame += x * 5;
	if (KeyFrame > 6.2831) KeyFrame = 0;

	IdleLoc = (std::sin(KeyFrame)) * DestRect.h / 6;
	DestRect.y = std::round(caty + IdleLoc);
}