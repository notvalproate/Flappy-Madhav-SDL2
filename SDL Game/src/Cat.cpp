#include "Cat.hpp"
#include <iostream>
#include <cmath>

Cat::Cat(SDL_Renderer* Ren, const int& width, const int& height) {
	//Load the cat and deadcat's textures, initialize width and height of the cat's rectangle
	CatTex = Texture::LoadTexture("assets/textures/cat.png", Ren);
	DeadCatTex = Texture::LoadTexture("assets/textures/deadcat.png", Ren);
	Renderer = Ren;
	DestRect.w = height / 15;
	DestRect.h = DestRect.w;

	//Place cat in the centre of the screen
	CenterY = height / 2;
	DestRect.x = width / 2 - (DestRect.w / 2);
	DestRect.y = CenterY - (DestRect.h / 2);
	CatY = DestRect.y;
	
	Velocity = 0;
	VelFac = -12;
	Gravity = DestRect.h * 45; //Gravity that the cat will experience
	KeyFrame = 0; //Argument for the SIN function for IDLE animation

	State = Ready; //Set cat's state to Ready (Menu)
}

Cat::~Cat() {
	SDL_DestroyTexture(CatTex);
	SDL_DestroyTexture(DeadCatTex);
}

bool Cat::HandleEvents(const SDL_Event& Event) {
	bool EventOccured = false;

	switch (Event.type) {
	case SDL_MOUSEBUTTONDOWN:
		EventOccured = true;
	case SDL_KEYDOWN:
		switch (Event.key.keysym.sym) {
		case SDLK_SPACE: case SDLK_UP:
			EventOccured = true;
		}
	}

	if (EventOccured) {
		if (State != Dead) {
			Jump();
		}
		else {
			ResetCat();
		}
	}

	return EventOccured;
}

void Cat::ResetCat() {
	//Reset Cat's Y position to centre of the screen, and set state to ready.
	DestRect.y = CenterY - (DestRect.h / 2);
	CatY = DestRect.y;
	Velocity = 0;
	KeyFrame = 0;
	State = Ready;
}

void Cat::Jump() {
	if(State == Ready) { //As Jump() is used to start the game as well as to jump.
		State = Alive;
	}

	Velocity = DestRect.h * VelFac; //Set the Cat's velocity to a value upwards for simulating a jump
}

void Cat::SetMode(const GameMode& mode) {
	Mode = mode;
	switch (Mode) {
	case Normal:
		VelFac = -12;
		break;
	case Speedy:
		VelFac = -9.5;
		break;
	}
}


void Cat::ApplyGravity(const float& DeltaTime) {
	Velocity += (Gravity * DeltaTime) / static_cast<float>(1000); //Increase velocity downward based on gravity
	CatY += (Velocity * DeltaTime) / static_cast <float>(1000); //Move the cat in the direction of that velocity
	DestRect.y = std::round(CatY); 
}

bool Cat::Update(const float& DeltaTime, Map* TheMap,bool& DeathDelay) {	
	switch (State) {
		case Ready:
			Idle(DeltaTime); //Play idle animation if cat is Ready
			return false;

		case Alive:
			//Check for collisions when the cat is alive, CheckCollisions() returns true if collided
			if (TheMap->CheckCollision(DestRect.x, DestRect.y, DestRect.w, DestRect.h)) {
				DeathDelay = true; //Set the delay for 500ms to start
				Velocity = DestRect.h * -6; //Set cat velocity a bit upwards on death
				State = Dead;
				return true;
			}
			break;
	}

	//Apply gravity on cat if it is Alive/Dead. Until it goes below the screen,
	if (DestRect.y <= CenterY * 2) ApplyGravity(DeltaTime);

	//Return false on default because no collision
	return false;
}

void Cat::Render() {
	switch (State) {
		case Dead:
			SDL_RenderCopy(Renderer, DeadCatTex, NULL, &DestRect); //Render Dead cat texture if dead
			break;
		default:
			SDL_RenderCopy(Renderer, CatTex, NULL, &DestRect); //Render normal texture on default
			break;
	}
}

void Cat::Idle(const float& DeltaTime) {
	float x = DeltaTime / static_cast<float>(1000); 
	KeyFrame += x * 5;
	if (KeyFrame > 6.2831) KeyFrame = 0; //If keyframe value exceeds 2pi, reset it to 0

	DestRect.y = CenterY + ((std::sin(KeyFrame)) * DestRect.h / 6); //Add that offset to the cat's position and round off
}