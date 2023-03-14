#include "Score.hpp"
#include "Texture.hpp"
#include <iostream>
#include <fstream>
#include <string>

Score::Score() { }

Score::Score(const char* texpath, SDL_Renderer* Ren, const int& width, const int& height) {
	//Initialize starting position to render Score
	DestRect.x = height / 20;
	DestRect.y = height / 20;
	DestRect.h = height / 10;
	ResetX = DestRect.x;

	//Digits vector to keep track of each digit, and count. both start at 0
	Digits.push_back(0);
	Count = 0;
	Renderer = Ren;
	NumTex = Texture::LoadTexture(texpath, Ren); //Loading texture that contains all digits

	GetSrc(); //Function to set the SrcRect for each digit
}

Score::~Score() { 
	SDL_DestroyTexture(NumTex);
}

void Score::GetSrc() {
	//Start with adding 0's Source rectangle
	NumSrc[0].y = 0;
	NumSrc[0].h = 10;
	NumSrc[0].x = 0;
	NumSrc[0].w = 7;

	//Every subsequent rectangle's x value, will be the x value of previous digit plus its width
	//Y value and height are same for all
	for (int i = 1; i < 10; i++) {
		NumSrc[i].y = 0;
		NumSrc[i].h = 10;
		NumSrc[i].x = NumSrc[i - 1].w + NumSrc[i - 1].x;

		//Only 1's width is 5 pixel, every other digit's width is 7 pixels
		if (i == 1) {
			NumSrc[i].w = 5;
			continue;
		}
		NumSrc[i].w = 7;
	}
}

void Score::GetDigits() {
	//Clears the Digits vector and gets each digit of Count
	int k = 1, i;
	Digits.clear();

	//If zero
	if (Count == 0) {
		Digits.push_back(0);
		return;
	}

	//Add digits iteratively
	while (Count / k != 0) {
		i = (Count / k) % 10;
		Digits.push_back(i);
		k *= 10;
	}
}

void Score::Increment() {
	//Increment the count and get the digits of the new score
	Count++;
	GetDigits();
}

void Score::Render() {
	//Output digits from left to right, new X is += previous digits width + 1 pixel gap
	for (int i = Digits.size() - 1; i >= 0; i--) {
		//For all digits except 1, width is 7 pixels. For 1, width is 5 pixels
		if (Digits[i] == 1) {
			DestRect.w = DestRect.h / 2;
		}
		else {
			DestRect.w = DestRect.h * ((float)7 / (float)10);
		}
		SDL_RenderCopy(Renderer, NumTex, &NumSrc[Digits[i]], &DestRect);
		DestRect.x += DestRect.w + 1;
	}
	//Reset the x value to the beginning for next render
	DestRect.x = ResetX;
}


HighScore::HighScore(const char* texpath, const char* startex, SDL_Renderer* Ren, const int& width, const int& height) {
	//Set destination rectangle, except width, which is calculated on render
	DestRect.x = (width - (height / 20)) + 1;
	DestRect.y = height / 20;
	DestRect.h = height / 10; 
	ResetX = DestRect.x;

	Mode = Normal;

	//Read the highscore from hs.dat
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
	//Output digits from right to left
	for (int i = 0; i < Digits.size(); i++) {
		//Same logic as render for score
		if (Digits[i] == 1) {
			DestRect.w = DestRect.h / 2;
		}
		else {
			DestRect.w = DestRect.h * ((float)7 / (float)10);
		}
		DestRect.x -= DestRect.w + 1;
		SDL_RenderCopy(Renderer, NumTex, &NumSrc[Digits[i]], &DestRect);
	}

	//set rect as a square, add 5 pixels to the right and render the star
	DestRect.w = DestRect.h;
	DestRect.x -= (DestRect.w + 5);
	SDL_RenderCopy(Renderer, StarTex, NULL, &DestRect);

	//Reset the x value
	DestRect.x = ResetX;
}

void HighScore::Read() {
	//Open hs.dat to read
	long long hs;
	std::ifstream os("assets/hs.dat", std::ios::binary);

	//If mode is speedy, offset and then read
	if (Mode == Speedy) os.seekg(sizeof(hs), std::ios::beg);
	os.read((char*)&hs, sizeof(hs));
	os.close();
	Count = hs - 4782423574423854;
	//Convert the count to digits
	GetDigits();
}

void HighScore::Write() {
	//Open hs.dat to write
	long long hs = Count + 4782423574423854;
	std::ofstream os("assets/hs.dat", std::ios::binary | std::ios::in | std::ios::out);

	//If mode is speedy, offset and then write
	if (Mode == Speedy) os.seekp(sizeof(hs), std::ios::beg);
	os.write((char*)&hs, sizeof(hs));
	os.close();
}

void HighScore::SetMode(const GameMode& mode) {
	//change mode and then RE-read the score
	Mode = mode;
	Read();
}