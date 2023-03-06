#pragma once
#include "SDL.h"
#include "Score.hpp"

class Pipe {
public:
	Pipe(const char* toptex, const char* bottomtex, SDL_Renderer* Ren, const int& width, const int& height, const int& offset);
	~Pipe();

	bool Update(const int& DeltaTime, const bool& Dead, Score* Count);
	void Render();
	void ResetPipe(const int& offset);
	bool CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath);

	bool next;
private:
	int xpos, ypos;
	int Velocity;
	SDL_Rect Top, Bottom;
	SDL_Texture* TopTex, * BottomTex;
	SDL_Renderer* Renderer;
};