#pragma once
#include "Pipe.hpp"
#include "Floor.hpp"
#include "Score.hpp"
#include "Audio.hpp"
#include <vector>
#include "GameModes.hpp"

class Map {
public:
	Map(SDL_Renderer* Ren, const int& width, const int& height, const int& veln, const int& vels);
	~Map();

	void Update(const float& DeltaTime, Audio* Point);
	void Render();
	void ResetMap();
	void SetMode(const GameMode& mode);
	bool CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath);

private:
	int MapHeight;
	Score* Count;
	HighScore* HS;
	SDL_Renderer* Renderer;
	Floor* F;
	Pipe* P[4];
	GameMode Mode;
};