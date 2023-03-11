#include "Pipe.hpp"
#include "Texture.hpp"
#include <cmath>
#include <iostream>
#include <random>

Pipe::Pipe(const char* toptex, const char* bottomtex, SDL_Renderer* Ren, const int& width, const int& height, const int& offset, const int& vel) {
	Renderer = Ren;
	//Load Top and bottom pipe textures
	TopTex = Texture::LoadTexture(toptex, Renderer);
	BottomTex = Texture::LoadTexture(bottomtex, Renderer);
	Velocity = vel;
	//Set state as not yet passed middle of screen
	passed = false;

	ScreenW = width;
	ScreenH = height;
	ScoreLine = (ScreenW / 2) - (ScreenH / 30); //X value that the pipe has to pass for score to increment

	//Point to get the ratio of the width and height of the textures
	SDL_Point TopSize, BottomSize;
	SDL_QueryTexture(TopTex, NULL, NULL, &TopSize.x, &TopSize.y);
	SDL_QueryTexture(BottomTex, NULL, NULL, &BottomSize.x, &BottomSize.y);

	Top.w = width / 10;
	Top.h = (int) (Top.w * ((float) TopSize.y / (float) TopSize.x));
	Bottom.w = Top.w;
	Bottom.h = (int) (Bottom.w * ((float) BottomSize.y / (float)BottomSize.x));
	
	SetPipeGap(offset); //Function to set the gap between the pipe at random height
}

Pipe::~Pipe() {
	SDL_DestroyTexture(TopTex);
	SDL_DestroyTexture(BottomTex);
}

bool Pipe::Update(const float& DeltaTime) {
	//Calculate how much distance pipe has moved left and Set the top and bottom pipes to same distance
	PipeX -= (Velocity * DeltaTime) / static_cast <float> (1000);
	Top.x = std::round(PipeX);
	Bottom.x = std::round(PipeX);

	//If pipe exits screen on right, reset it
	if (Top.x + Top.w <= 0) {
		passed = false;
		SetPipeGap(0); //Passing offset as 0 resets the pipe just to the right of the screen
	}

	//Check if pipe passes the scoreline
	if (!passed && Top.x + Top.w <= ScoreLine) {
		passed = true;
		return true; //Returns true if passed, to increment score
	}

	//Return false if not passed
	return false;
}

void Pipe::Render() {
	SDL_RenderCopy(Renderer, TopTex, NULL, &Top);
	SDL_RenderCopy(Renderer, BottomTex, NULL, &Bottom);
}

void Pipe::ResetPipe(const int& offset) {
	//Reset all the pipes
	passed = false;
	SetPipeGap(offset);
}

bool Pipe::CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath) {
	//Axis Aligned Collision Check
	if (
		Top.x < catx + catw &&
		Top.x + Top.w > catx &&
		Top.y < caty + cath &&
		Top.y + Top.h > caty ||
		Bottom.x < catx + catw &&
		Bottom.x + Bottom.w > catx &&
		Bottom.y < caty + cath &&
		Bottom.y + Bottom.h > caty
		) {
		return true;
	}

	return false;
}

void Pipe::SetPipeGap(const int& offset) {
	//Generate a random number between 0 to 8
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 8);

	//Minimum height from top is 1/10th of screen height, then + random multiple of ScreenHeight / 18
	int bottom = (ScreenH / 10) + (dist(gen) * (ScreenH / 18));
	Top.y = bottom - Top.h;
	Bottom.y = bottom + (ScreenW / 8);

	//The X position of each pipe is chosen with an offset in the beginning of the game. 
	PipeX = ScreenW + offset * (ScreenW + Top.w) / 4;
	Top.x = std::round(PipeX);
	Bottom.x = std::round(PipeX);
}