#include "Pipe.hpp"
#include "Texture.hpp"
#include <iostream>
#include <random>

Pipe::Pipe(const char* toptex, const char* bottomtex, SDL_Renderer* Ren, const int& width, const int& height, const int& offset) {
	Renderer = Ren;
	TopTex = Texture::LoadTexture(toptex, Renderer);
	BottomTex = Texture::LoadTexture(bottomtex, Renderer);
	Velocity = width / 4;
	if (offset == 0) { 
		next = true;
	}
	else {
		next = false;
	}

	xpos = width;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 7);
	ypos = height;
	int bottom = (height / 10) + (dist(gen) * (height / 18));

	SDL_Point TopSize, BottomSize;
	SDL_QueryTexture(TopTex, NULL, NULL, &TopSize.x, &TopSize.y);
	SDL_QueryTexture(BottomTex, NULL, NULL, &BottomSize.x, &BottomSize.y);

	Top.w = width / 10;
	Top.h = Top.w * ((float) TopSize.y / (float) TopSize.x);
	Top.x = xpos + offset * (width + Top.w) / 4;
	Top.y = bottom - Top.h;

	Bottom.w = Top.w;
	Bottom.h = Bottom.w * ((float)BottomSize.y / (float)BottomSize.x);
	Bottom.x = xpos + offset * (width + Top.w) / 4;
	Bottom.y = bottom + (width / 8);
}

Pipe::~Pipe() {
	SDL_DestroyTexture(TopTex);
	SDL_DestroyTexture(BottomTex);
}

bool Pipe::Update(const int& DeltaTime, const bool& Dead, Score* Count) {
	if (Dead) {
		return false;
	}

	int Distance = Velocity * (DeltaTime / 1000.0);
	Top.x -= Distance;
	Bottom.x -= Distance;

	if (Top.x + Top.w <= 0) {
		Top.x = xpos;
		Bottom.x = xpos;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, 8);
		int bottom = (ypos / 10) + (dist(gen) * (ypos / 18));
		Top.y = bottom - Top.h;
		Bottom.y = bottom + (xpos / 8);
	}

	if (next && Top.x + Top.w <= xpos / 2) {
		Count->Increment();
		next = false;
		return true;
	}

	return false;
}

void Pipe::Render() {
	SDL_RenderCopy(Renderer, TopTex, NULL, &Top);
	SDL_RenderCopy(Renderer, BottomTex, NULL, &Bottom);
}

void Pipe::ResetPipe(const int& offset) {
	if (offset == 0) {
		next = true; 
	}
	else {
		next = false;
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 8);
	int bottom = (ypos / 10) + (dist(gen) * (ypos / 18));

	Top.y = bottom - Top.h;
	Bottom.y = bottom + (xpos / 8);
	Top.x = xpos + offset * (xpos + Top.w) / 4;
	Bottom.x = xpos + offset * (xpos + Top.w) / 4;
}

bool Pipe::CheckCollision(const int& catx, const int& caty, const int& catw, const int& cath) {
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