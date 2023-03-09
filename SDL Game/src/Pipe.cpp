#include "Pipe.hpp"
#include "Texture.hpp"
#include <cmath>
#include <iostream>
#include <random>

Pipe::Pipe(const char* toptex, const char* bottomtex, SDL_Renderer* Ren, const int& width, const int& height, const int& offset) {
	Renderer = Ren;
	TopTex = Texture::LoadTexture(toptex, Renderer);
	BottomTex = Texture::LoadTexture(bottomtex, Renderer);
	Velocity = width / 4.5;
	passed = false;
	ScreenW = width;
	ScreenH = height;
	ScoreLine = (ScreenW / 2) - (ScreenH / 30);

	SDL_Point TopSize, BottomSize;
	SDL_QueryTexture(TopTex, NULL, NULL, &TopSize.x, &TopSize.y);
	SDL_QueryTexture(BottomTex, NULL, NULL, &BottomSize.x, &BottomSize.y);

	Top.w = width / 10;
	Top.h = (int) (Top.w * ((float) TopSize.y / (float) TopSize.x));
	Bottom.w = Top.w;
	Bottom.h = (int) (Bottom.w * ((float)BottomSize.y / (float)BottomSize.x));

	SetPipeGap(offset);
}

Pipe::~Pipe() {
	SDL_DestroyTexture(TopTex);
	SDL_DestroyTexture(BottomTex);
}

bool Pipe::Update(const int& DeltaTime) {
	Distance -= static_cast<float> (Velocity * DeltaTime) / static_cast < float> (1000);
	Top.x = (int) std::round(Distance);
	Bottom.x = (int) std::round(Distance);

	if (Top.x + Top.w <= 0) {
		passed = false;
		SetPipeGap(0);
	}

	if (!passed && Top.x + Top.w <= ScoreLine) {
		passed = true;
		return true;
	}

	return false;
}

void Pipe::Render() {
	SDL_RenderCopy(Renderer, TopTex, NULL, &Top);
	SDL_RenderCopy(Renderer, BottomTex, NULL, &Bottom);
}

void Pipe::ResetPipe(const int& offset) {
	passed = false;
	SetPipeGap(offset);
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

void Pipe::SetPipeGap(const int& offset) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 8);
	int bottom = (ScreenH / 10) + (dist(gen) * (ScreenH / 18));
	Top.y = bottom - Top.h;
	Bottom.y = bottom + (ScreenW / 8);

	Top.x = ScreenW + offset * (ScreenW + Top.w) / 4;
	Bottom.x = ScreenW + offset * (ScreenW + Top.w) / 4;
	Distance = (float) Bottom.x;
}