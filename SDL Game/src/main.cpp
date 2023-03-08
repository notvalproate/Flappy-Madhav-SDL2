#include "Game.hpp"
#include "Cat.hpp"
#include <iostream>
 
int main(int argc, char* argv[]) {
	
	Game game;
	game.Init("Flappy Madhav", "assets/textures/icon.png", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);
	
	int LastFrame = 0;
	int DeltaTime = 0;

	while (game.Exit()) {
		DeltaTime = SDL_GetTicks() - LastFrame;

		if (DeltaTime >= game.GetDelta()) {
			game.HandleEvents(DeltaTime);
			game.Update(DeltaTime);
			game.Render();
			LastFrame = SDL_GetTicks();
		}

		SDL_Delay(1);
	}

	game.Clean();

	return 0;
}