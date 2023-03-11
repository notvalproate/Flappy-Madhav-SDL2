#pragma once
#include "SDL.h"
#include "Texture.hpp"
#include "Map.hpp"

enum CatState {
	Ready,
	Alive,
	Dead
};

class Cat {
public:
	Cat(SDL_Renderer* Ren, const int& width, const int& height);
	~Cat();
	
	void Jump();
	bool Update(const float& DeltaTime, Map* TheMap, bool& DeathDelay);
	void Render();
	void ResetCat();

private:
	CatState State;
	float CenterY, CatY;
	float Gravity;
	float Velocity;
	float KeyFrame;
	SDL_Texture *CatTex, *DeadCatTex;
	SDL_Rect DestRect;
	SDL_Renderer* Renderer;

	void ApplyGravity(const float& DeltaTime);
	void Idle(const float& DeltaTime);
};