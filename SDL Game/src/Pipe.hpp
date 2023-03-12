#pragma once
#include "SDL.h"
#include "Score.hpp"
#include "GameModes.hpp"

class Pipe {
public:
	Pipe(const char* toptex, const char* bottomtex, SDL_Renderer* Ren, const int& width, const int& height, const int& offset, const int& veln, const int& vels, const int& previousy);
	~Pipe();

	bool Update(const float& DeltaTime, const int& previousy);
	void Render();
	void ResetPipe(const int& offset, const int& previousy);
	void SetMode(const GameMode& mode, const int& offset, const int& previousy);
	bool CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath);
	int GetYPos() { return Top.y + Top.h; }

private:
	bool passed;
	int ScreenW, ScreenH, ScoreLine;
	int VelocityN, VelocityS, CurrVel;
	float PipeX;
	SDL_Rect Top, Bottom;
	SDL_Texture *TopTex, *BottomTex;
	SDL_Renderer* Renderer;
	GameMode Mode;

	void SetPipeGap(const int& offset, const int& previousy);
};