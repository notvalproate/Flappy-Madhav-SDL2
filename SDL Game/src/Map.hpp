#pragma once
#include "Pipe.hpp"
#include "Floor.hpp"
#include "Score.hpp"
#include "Audio.hpp"
#include <vector>

class Map {
public:
	Map(SDL_Renderer* Ren, const int& width, const int& height);
	~Map();

	void Update(const int& DeltaTime, const bool& Dead, Audio* Point);
	void Render();
	void ResetMap();
	bool CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath);

private:
	Score* Count;
	SDL_Renderer* Renderer;
	Floor* F;
	Pipe* P[4];
};