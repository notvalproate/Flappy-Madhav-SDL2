#pragma once
#include "SDL.h"
#include <vector>

class Score {
public:
	Score(const char* texpath, SDL_Renderer* Ren, const int& width, const int& height);
	~Score();

	void Render();
	void Increment();
	void Reset() { Count = 0; Digits.clear(); Digits.push_back(0); }

private:
	int Count;
	std::vector<int> Digits;
	SDL_Renderer* Renderer;
	SDL_Texture* NumTex;
	SDL_Rect NumSrc[10], DestRect;
};