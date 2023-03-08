#include "Map.hpp"
#include <iostream>

Map::Map(SDL_Renderer* Ren, const int& width, const int& height) {
	Renderer = Ren;
	Count = new Score("assets/textures/numbers.png", Ren, width, height);
	F = new Floor("assets/textures/floor.png", Renderer, width, height);
	HS = new HighScore("assets/textures/numbers.png", "assets/textures/star.png", Ren, width, height);
	for (int i = 0; i < 4; i++) {
		P[i] = new Pipe("assets/textures/top_pipe.png", "assets/textures/bottom_pipe.png", Renderer, width, height, i);
	}
}

Map::~Map() { 
	free(HS);
	free(Count);
	free(F);
	free(P[0]);
	free(P[1]);
	free(P[2]);
	free(P[3]);
}

void Map::Update(const int& DeltaTime, Audio* Point) {
	for (int i = 0; i < 4; i++) {
		if (P[i]->Update(DeltaTime)) {
			Point->PlaySound();
			Count->Increment();
			if (Count->GetCount() > HS->GetCount()) {
				HS->Increment(); 
				HS->SetUpdate();
			}
		}
	}
	F->Update(DeltaTime);
}

void Map::Render() {
	for (int i = 0; i < 4; i++) {
		P[i]->Render();
	}
	F->Render();
	Count->Render();
	HS->Render();
}

void Map::ResetMap() {
	Count->Reset();
	for (int i = 0; i < 4; i++) {
		P[i]->ResetPipe(i);
	}
}

bool Map::CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath) {
	if (P[Count->GetCount() % 4]->CheckCollision(catx, caty, catw, cath)) {
			HS->Write();
			return true;
	};

	if (caty + cath >= F->GetHeight() || caty <= 0) {
		HS->Write();
		return true;
	}

	return false;
}