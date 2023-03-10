#include "Game.hpp"
#include "Cat.hpp"
#include <iostream>
 
int main(int argc, char* argv[]) {
	
	//Initialize Game Object
	Game game;
	game.Init("Flappy Madhav", "assets/textures/icon.png", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	
	//Variables for Limiting Frame-Rate
	Uint32 LastFrame = 0;
	int DeltaTime = 0;

	while (game.Exit()) {
		//Get the delta time since last frame
		DeltaTime = SDL_GetTicks() - LastFrame;

		if (DeltaTime >= game.GetDelta()) {
			//Delta time passed for consistent game speed for different framerates
			game.HandleEvents(DeltaTime); 
			game.Update(DeltaTime);
			game.Render();
			//Get the time in ticks of the frame was rendered at
			LastFrame = SDL_GetTicks();
		}

		//Apply Delay to limit Loop interations per second to ~1000
		SDL_Delay(1);
	}

	//Clean the game object
	game.Clean();

	return 0;
}