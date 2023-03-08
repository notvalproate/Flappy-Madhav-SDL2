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
	bool Update(const int& DeltaTime, Map* TheMap, bool& DeathDelay);
	void Render();
	void ResetCat();

private:
	CatState State;
	int x, y;
	float caty;
	float Gravity;
	float Velocity;
	float KeyFrame, IdleLoc;
	SDL_Texture *CatTex, *DeadCatTex;
	SDL_Rect DestRect;
	SDL_Renderer* Renderer;

	void ApplyGravity(const int& DeltaTime);
	void Idle(const int& DeltaTime);
};