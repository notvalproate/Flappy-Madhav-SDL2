#include "Score.hpp"
#include "Texture.hpp"
#include <iostream>
#include <fstream>

Score::Score() { }

Score::Score(const char* texpath, SDL_Renderer* Ren, const int& width, const int& height) {
	DestRect.x = height / 20;
	DestRect.y = height / 20;
	DestRect.h = height / 10;
	xpos = DestRect.x;

	Digits.push_back(0);
	Count = 0;
	Renderer = Ren;
	NumTex = Texture::LoadTexture(texpath, Ren);

	GetSrc();
}

Score::~Score() { 
	SDL_DestroyTexture(NumTex);
}

void Score::GetSrc() {
	NumSrc[0].y = 0;
	NumSrc[0].h = 10;
	NumSrc[0].x = 0;
	NumSrc[0].w = 7;

	for (int i = 1; i < 10; i++) {
		NumSrc[i].y = 0;
		NumSrc[i].h = 10;
		NumSrc[i].x = NumSrc[i - 1].w + NumSrc[i - 1].x;
		if (i == 1) {
			NumSrc[i].w = 5;
		}
		else {
			NumSrc[i].w = 7;
		}
	}
}

void Score::GetDigits() {
	int k = 1, i;
	Digits.clear();
	while (Count / k != 0 || k == 1) {
		i = (Count / k) % 10;
		Digits.push_back(i);
		k *= 10;
	}
}

void Score::Increment() {
	Count++;
	GetDigits();
}

void Score::Render() {
	for (int i = Digits.size() - 1; i >= 0; i--) {
		if (Digits[i] == 1) {
			DestRect.w = DestRect.h / 2;
		}
		else {
			DestRect.w = DestRect.h * ((float)7 / (float)10);
		}
		SDL_RenderCopy(Renderer, NumTex, &NumSrc[Digits[i]], &DestRect);
		DestRect.x += DestRect.w + 1;
	}

	DestRect.x = xpos;
}


HighScore::HighScore(const char* texpath, const char* startex, SDL_Renderer* Ren, const int& width, const int& height) {
	DestRect.x = (width - (height / 20)) + 1;
	DestRect.y = height / 20;
	DestRect.h = height / 10; \
	xpos = DestRect.x;

	Read();
	Renderer = Ren;
	NumTex = Texture::LoadTexture(texpath, Ren);
	StarTex = Texture::LoadTexture(startex, Ren);
	GetSrc();
}

HighScore::~HighScore() { 
	SDL_DestroyTexture(NumTex);
	SDL_DestroyTexture(StarTex);
}

void HighScore::Render() {
	for (int i = 0; i < Digits.size(); i++) {
		if (Digits[i] == 1) {
			DestRect.w = DestRect.h / 2;
		}
		else {
			DestRect.w = DestRect.h * ((float)7 / (float)10);
		}
		DestRect.x -= DestRect.w + 1;
		SDL_RenderCopy(Renderer, NumTex, &NumSrc[Digits[i]], &DestRect);
	}

	DestRect.w = DestRect.h;
	DestRect.x -= (DestRect.w + 5);
	SDL_RenderCopy(Renderer, StarTex, NULL, &DestRect);

	DestRect.x = xpos;
}

void HighScore::Read() {
	long long hs;
	std::ifstream os("assets/hs.dat", std::ios::binary);
	os.read((char*)&hs, sizeof(hs));
	os.close();
	Count = hs - 4782423574423854;

	GetDigits();
}

void HighScore::Write() {
	long long hs = Count + 4782423574423854;
	std::ofstream os("assets/hs.dat", std::ios::binary);
	os.write((char*)&hs, sizeof(hs));
	os.close();
}