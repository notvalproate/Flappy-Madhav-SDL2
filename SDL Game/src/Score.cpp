#include "Score.hpp"
#include "Texture.hpp"
#include <iostream>

Score::Score(const char* texpath, SDL_Renderer* Ren, const int& width, const int& height) {
	DestRect.x = height / 20;
	DestRect.y = height / 20;
	DestRect.h = height / 10;

	Digits.push_back(0);
	Count = 0;
	Renderer = Ren;
	NumTex = Texture::LoadTexture(texpath, Ren);

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

Score::~Score() { 
	SDL_DestroyTexture(NumTex);
}

void Score::Increment() {
	Count++;

	int k = 1, i = 0;

	Digits.clear();
	while (Count / k != 0) {
		Digits.push_back((Count / k) % 10); 
		i++;
		k *= 10;
	}
}

void Score::Render() {
	int reset = DestRect.x;
	DestRect.w = DestRect.h * ((float) 7 / (float) 10);
	if (Digits[Digits.size() - 1] == 1) {
		DestRect.w = DestRect.h / 2;
	}
	SDL_RenderCopy(Renderer, NumTex, &NumSrc[Digits[Digits.size() - 1]], &DestRect);
	
	for (int i = Digits.size() - 2; i >= 0; i--) {
		DestRect.x += DestRect.w + 1;
		if (Digits[i] == 1) {
			DestRect.w = DestRect.h / 2;
		}
		else {
			DestRect.w = DestRect.h * ((float)7 / (float)10);
		}
		SDL_RenderCopy(Renderer, NumTex, &NumSrc[Digits[i]], &DestRect);
	}

	DestRect.x = reset;
}