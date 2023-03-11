#include "Game.hpp"
#include "Cat.hpp"
#include <iostream>
 
int main(int argc, char* argv[]) {
	
	//Initialize Game Object
	Game game;
	game.Init("Flappy Madhav", "assets/textures/icon.png", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);

	while (game.Exit()) {
			game.HandleEvents(); 
			game.Update();
			game.Render();
	}

	//Clean the game object
	game.Clean();

	return 0;
}