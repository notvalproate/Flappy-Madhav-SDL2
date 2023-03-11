#pragma once
#include "SDL.h"

class Floor {
public:
	Floor(const char* texturepath, SDL_Renderer* Ren, const int& width, const int& height, const int& vel);
	~Floor();

	void Update(const float& DeltaTime);
	void Render();
	inline int GetHeight() { return DestRect.y; }

private:
	int Velocity;
	float FloorX;
	SDL_Rect DestRect;
	SDL_Texture* FloorTex;
	SDL_Renderer* Renderer;

	void ResetFloor();
};