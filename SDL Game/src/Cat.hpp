#pragma once
#include "SDL.h"
#include "Texture.hpp"
#include "Map.hpp"
#include "GameModes.hpp"

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
	bool HandleEvents(const SDL_Event& Event);
	void Render();
	void SetMode(const GameMode& mode);

private:
	CatState State;
	GameMode Mode;
	float CenterY, CatY;
	float Gravity;
	float Velocity, VelFac;
	float KeyFrame;
	SDL_Texture *CatTex, *DeadCatTex;
	SDL_Rect DestRect;
	SDL_Renderer* Renderer;

	void ApplyGravity(const float& DeltaTime);
	void Idle(const float& DeltaTime);
	void ResetCat();
};