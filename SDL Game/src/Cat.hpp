#pragma once
#include "SDL.h"
#include "Texture.hpp"
#include "Map.hpp"
#include "Audio.hpp"
#include <vector>

class Cat {
public:
	Cat(const char* texturepath, SDL_Renderer* Ren, const int& width, const int& height);
	~Cat();

	void Jump(Map* TheMap, Audio* JumpSound);
	void Update(const int& DeltaTime, const int& height, Map* TheMap, Audio* DeathSound);
	void Render();
	bool GetDeath() { return Dead; }
	bool GetStart() { return Start; }

private:
	int x, y;
	float Gravity;
	float Velocity;
	bool Dead;
	bool Start;
	SDL_Texture* CatTex;
	SDL_Rect DestRect;
	SDL_Renderer* Renderer;
};