#include "Map.hpp"
#include <iostream>

Map::Map(SDL_Renderer* Ren, const int& width, const int& height) {
	Renderer = Ren;
	Count = new Score("assets/textures/numbers.png", Ren, width, height);
	F = new Floor("assets/textures/floor.png", Renderer, width, height);
	for (int i = 0; i < 4; i++) {
		P[i] = new Pipe("assets/textures/top_pipe.png", "assets/textures/bottom_pipe.png", Renderer, width, height, i);
	}
}

Map::~Map() { 
	free(Count);
	free(F);
	free(P[0]);
	free(P[1]);
	free(P[2]);
	free(P[3]);
}

void Map::Update(const int& DeltaTime, const bool& Dead, Audio* Point) {
	for (int i = 0; i < 4; i++) {
		if (P[i]->Update(DeltaTime, Dead, Count)) {
			P[(i + 1) % 4]->next = true;
			Point->PlaySound();
		}
	}
	F->Update(DeltaTime, Dead);
}

void Map::Render() {
	for (int i = 0; i < 4; i++) {
		P[i]->Render();
	}
	F->Render();
	Count->Render();
}

void Map::ResetMap() {
	Count->Reset();
	for (int i = 0; i < 4; i++) {
		P[i]->ResetPipe(i);
	}
}

bool Map::CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath) {
	for (int i = 0; i < 4; i++) {
		if (P[i]->next) {
			if (P[i]->CheckCollision(catx, caty, catw, cath)) {
				return true;
			};
			break;
		}
	}
	if (caty + cath >= F->GetHeight() || caty <= 0) {
		return true;
	}

	return false;
}