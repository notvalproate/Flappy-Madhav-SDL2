#pragma once
#include "SDL.h"
#include "GameModes.hpp"

class Floor {
public:
	Floor(const char* texturepath, SDL_Renderer* Ren, const int& width, const int& height, const int& veln, const int& vels);
	~Floor();

	void Update(const float& DeltaTime);
	void Render();
	void SetMode(const GameMode& mode);
	inline int GetHeight() { return DestRect.y; }

private:
	int VelocityN, VelocityS, CurrVel;
	float FloorX;
	SDL_Rect DestRect;
	SDL_Texture* FloorTex;
	SDL_Renderer* Renderer;
	GameMode Mode;

	void ResetFloor();
};