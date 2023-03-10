#include "Floor.hpp"
#include "Texture.hpp"
#include <cmath>

Floor::Floor(const char* texturepath, SDL_Renderer* Ren, const int& width, const int& height, const int& vel) {
	Renderer = Ren;

	//Load floor's texture
	FloorTex = Texture::LoadTexture(texturepath, Ren);

	//Set the Source rectangle for the floor
	SDL_Point size;
	SDL_QueryTexture(FloorTex, NULL, NULL, &size.x, &size.y);
	SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = size.x;
	SrcRect.h = 10;

	//Set the Destination Rectangle for both first and second half of floor
	FirstHalf.y = (height * 9) / 10;
	FirstHalf.w = width;
	FirstHalf.h = height / 10;

	//Second half is placed outside the screen, just to the right of first half
	SecondHalf.y = FirstHalf.y;
	SecondHalf.w = FirstHalf.w;
	SecondHalf.h = FirstHalf.h;

	Position = 0; //Keeps track of floor's position in float

	ResetFloor();

	Velocity = vel; //Velocity at which the map will move towards the left;
}

Floor::~Floor() { 
	SDL_DestroyTexture(FloorTex);
}

void Floor::ResetFloor() {
	//Reset the floor, first half to the screen and second half on the right of it;
	Position = 0;
	FirstHalf.x = 0;
	SecondHalf.x = FirstHalf.w;
}

void Floor::Update(const int& DeltaTime) {
	Position -= (Velocity * DeltaTime) / static_cast <float> (1000); //Move the floor's position to the left based on velocity
	//Set the position to first half and second half
	FirstHalf.x = std::round(Position);
	SecondHalf.x = std::round(Position) + FirstHalf.w;

	//If second half reaches the left of the screen (No more floor to show on the right), reset the floor's position
	if (SecondHalf.x <= 0) {
		ResetFloor();
	}
}

void Floor::Render() {
	SDL_RenderCopy(Renderer, FloorTex, &SrcRect, &FirstHalf);
	SDL_RenderCopy(Renderer, FloorTex, &SrcRect, &SecondHalf);
}