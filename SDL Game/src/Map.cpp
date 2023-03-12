#include "Map.hpp"
#include <iostream>

Map::Map(SDL_Renderer* Ren, const int& width, const int& height, const int& veln, const int& vels) {
	Renderer = Ren;
	Mode = Normal;
	MapHeight = height;
	//Initialize Score, Highscore, Floor, and Pipe objects
	Count = new Score("assets/textures/numbers.png", Ren, width, height);
	F = new Floor("assets/textures/floor.png", Renderer, width, height, veln, vels);
	HS = new HighScore("assets/textures/numbers.png", "assets/textures/star.png", Ren, width, height);
	//Only 4 pipes as there are max of 4 pipes on the screen at once
	for (int i = 0; i < 4; i++) {
		std::cout << "Pipe" << i;
		P[i] = new Pipe("assets/textures/top_pipe.png", "assets/textures/bottom_pipe.png", Renderer, width, height, i, veln, vels, 0);
	}
}

Map::~Map() { 
	//Free all pointers
	free(HS);
	free(Count);
	free(F);
	free(P[0]);
	free(P[1]);
	free(P[2]);
	free(P[3]);
}

void Map::Update(const float& DeltaTime, Audio* Point) {
	//Update every pipe's position
	for (int i = 0; i < 4; i++) {
		if (P[i]->Update(DeltaTime, P[(3 + i) % 4]->GetYPos())) { //Update() returns true if the pipe passed the screen's halfway mark (Increment the Score)
			Point->PlaySound(); //Play point scored sound and increment score
			Count->Increment();
			if (Count->GetCount() > HS->GetCount()) { //If the score exceeded highscore, increment highscore as well
				HS->Increment(); 
			}
		}
	}
	//Update the floor's position
	F->Update(DeltaTime);
}

void Map::Render() {
	//Render each pipe and the floor and scores
	for (int i = 0; i < 4; i++) {
		P[i]->Render();
	}
	F->Render();
	Count->Render();
	HS->Render();
}

void Map::ResetMap() {
	//Reset the Score Count and the pipes positions
	Count->Reset();
	for (int i = 0; i < 4; i++) {
		P[i]->ResetPipe(i, P[(3 + i) % 4]->GetYPos());
	}
}

void Map::SetMode(const GameMode& mode) {
	//Set the gamemode for pipes, floor and the highscore.
	F->SetMode(mode);
	P[0]->SetMode(mode, 0, (3 * MapHeight) / 8);
	for (int i = 1; i < 4; i++) {
		P[i]->SetMode(mode, i, P[(3 + i) % 4]->GetYPos());
	}
	HS->SetMode(mode);
}

bool Map::CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath) {
	//Check for collisions of the next pipe. [Score mod 4] gives which pipe is the next pipe, as the pipes loop around 
	//Hence check collision only with the next pipe
	if (P[Count->GetCount() % 4]->CheckCollision(catx, caty, catw, cath)) {
			HS->Write(); //If collided, write the Highscore to the hs.dat file and return true
			return true;
	};

	//Checking collision with floor and doing the same
	if (caty + cath >= F->GetHeight() || caty <= 0) {
		HS->Write(); 
		return true;
	}

	//Return false if no collision detected
	return false;
}