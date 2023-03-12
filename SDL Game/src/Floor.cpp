#include "Floor.hpp"
#include "Texture.hpp"
#include <cmath>

Floor::Floor(const char* texturepath, SDL_Renderer* Ren, const int& width, const int& height, const int& veln, const int& vels) {
	Renderer = Ren;
	Mode = Normal;

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

	CurrVel = veln; //Velocity at which the map will move towards the left;
	VelocityN = veln;
	VelocityS = vels;
}

Floor::~Floor() { 
	SDL_DestroyTexture(FloorTex);
}

void Floor::ResetFloor() {
	//Reset the floor, first half to the screen and second half on the right of it;
	FloorX += DestRect.w / 2;
	DestRect.x = std::round(FloorX);
}

void Floor::SetMode(const GameMode& mode) {
	//set the mode and change the velocity based on the mode
	Mode = mode;

	switch (Mode) {
	case Normal:
		CurrVel = VelocityN;
		break;
	case Speedy:
		CurrVel = VelocityS;
		break;
	}
}

void Floor::Update(const float& DeltaTime) {
	FloorX -= (CurrVel * DeltaTime) / static_cast <float> (1000);
	DestRect.x = std::round(FloorX);  //Move the floor's position to the left based on velocity

	//If second half reaches the left of the screen (No more floor to show on the right), reset the floor's position
	if (DestRect.x + (DestRect.w / 2) <= 0) {
		ResetFloor();
	}
}

void Floor::Render() {
	SDL_RenderCopy(Renderer, FloorTex, NULL, &DestRect);
}