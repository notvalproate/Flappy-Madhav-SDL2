#include "Game.hpp"
#include "Cat.hpp"

int main(int argc, char* argv[]) {
	
	Game game;
	game.Init("Flappy Madhav", "assets/textures/icon.png", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720);

	Uint32 LastFrame = 0;
	int DeltaTime = 0;

	while (game.Exit()) {
		DeltaTime = SDL_GetTicks() - LastFrame;
		game.HandleEvents();

		if (DeltaTime > game.GetDelta()) {
			game.Update(DeltaTime);
			game.Render();
			LastFrame = SDL_GetTicks();
		}
	}

	game.Clean();

	return 0;
}