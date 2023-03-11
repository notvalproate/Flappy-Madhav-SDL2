#include "Floor.hpp"
#include "Texture.hpp"
#include <cmath>

Floor::Floor(const char* texturepath, SDL_Renderer* Ren, const int& width, const int& height, const int& vel) {
	Renderer = Ren;

	//Load floor's texture
	FloorTex = Texture::LoadTexture(texturepath, Ren);

	//Set the Destination Rectangle for both first and second half of floor
	SDL_Point size;
	SDL_QueryTexture(FloorTex, NULL, NULL, &size.x, &size.y);

	DestRect.y = (height * 9) / 10;
	DestRect.w = 2 * width;
	DestRect.h = height / 10;

	//Set Floor postions;
	FloorX = 0;
	DestRect.x = 0;

	Velocity = vel; //Velocity at which the map will move towards the left;
}

Floor::~Floor() { 
	SDL_DestroyTexture(FloorTex);
}

void Floor::ResetFloor() {
	//Reset the floor, first half to the screen and second half on the right of it;
	FloorX += DestRect.w / 2;
	DestRect.x = std::round(FloorX);
}

void Floor::Update(const float& DeltaTime) {
	FloorX -= (Velocity * DeltaTime) / static_cast <float> (1000);
	DestRect.x = std::round(FloorX);  //Move the floor's position to the left based on velocity

	//If second half reaches the left of the screen (No more floor to show on the right), reset the floor's position
	if (DestRect.x + (DestRect.w / 2) <= 0) {
		ResetFloor();
	}
}

void Floor::Render() {
	SDL_RenderCopy(Renderer, FloorTex, NULL, &DestRect);
}