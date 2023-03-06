#pragma once
#include "SDL.h"

class Floor {
public:
	Floor(const char* texturepath, SDL_Renderer* Ren, const int& width, const int& height);
	~Floor();

	void Update(const int& DeltaTime, const bool& Dead);
	void Render();
	int GetHeight() { return FirstHalf.y; }
private:
	int Velocity;
	SDL_Rect SrcRect, FirstHalf, SecondHalf;
	SDL_Texture* FloorTex;
	SDL_Renderer* Renderer;
};