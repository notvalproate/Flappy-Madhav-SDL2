#pragma once
#include "SDL.h"
#include <vector>

class Score {
public:
	Score();
	Score(const char* texpath, SDL_Renderer* Ren, const int& width, const int& height);
	~Score();

	virtual void Render();
	void Increment();
	int GetCount() { return Count; }
	void Reset() { Count = 0; Digits.clear(); Digits.push_back(0); }

protected:
	int Count, xpos;
	std::vector<int> Digits;
	SDL_Renderer* Renderer;
	SDL_Texture* NumTex;
	SDL_Rect NumSrc[10], DestRect;
	void GetDigits();
	void GetSrc();
};

class HighScore : public Score {
public:
	HighScore(const char* texpath, const char* startex, SDL_Renderer* Ren, const int& width, const int& height);
	~HighScore();

	void SetUpdate() { Update = true; }
	void Render();
	void Write();
private:
	bool Update;
	void Read();
	SDL_Texture* StarTex;
};