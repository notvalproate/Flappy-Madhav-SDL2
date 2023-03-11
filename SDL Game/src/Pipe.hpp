#pragma once
#include "SDL.h"
#include "Score.hpp"

class Pipe {
public:
	Pipe(const char* toptex, const char* bottomtex, SDL_Renderer* Ren, const int& width, const int& height, const int& offset, const int& vel);
	~Pipe();

	bool Update(const float& DeltaTime);
	void Render();
	void ResetPipe(const int& offset);
	bool CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath);

private:
	bool passed;
	int ScreenW, ScreenH, ScoreLine;
	int Velocity;
	float PipeX;
	SDL_Rect Top, Bottom;
	SDL_Texture *TopTex, *BottomTex;
	SDL_Renderer* Renderer;

	void SetPipeGap(const int& offset);
};