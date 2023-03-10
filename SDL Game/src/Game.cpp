#include "Game.hpp"
#include "SDL_mixer.h"
#include <iostream>

Game::Game() : IsRunning(false), FrameDelta(16), Window(nullptr), Renderer(nullptr), DeathDelay(false), DelayCount(0), State(Menu) { }
 
Game::~Game() { }

void Game::Init(const char* title, const char* iconpath, const int& x, const int& y, int width, int height) {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cout << "Error: Couldn't Initialize Subsystems..." << std::endl;
		return;
	}
	std::cout << "Stage: Initialized Subsystems..." << std::endl;

	//Get the user's display mode
	if (SDL_GetDisplayMode(0, 0, &Mode)) {
		std::cout << "Error: Couldn't Get Display Mode...Framerate set to 60" << std::endl;
	}
	else {
		std::cout << "Stage: Display Mode Initialized..." << std::endl;
		FrameDelta = 1000 / Mode.refresh_rate;
		//Set minimum frame-time to 1000 / refresh rate of the user's monitor.
	}

	//Width and Height of the screen
	w = width;
	h = height;

	//Create an SDL Window
	if (!(Window = SDL_CreateWindow(title, x, y, width, height, 0))) {
		std::cout << "Error: Couldn't Initialize Window..." << std::endl;
		return;
	}
	SDL_Surface* TempSurface = IMG_Load(iconpath);
	SDL_SetWindowIcon(Window, TempSurface); //Setting window icon
	SDL_FreeSurface(TempSurface);
	std::cout << "Stage: Initialized Window..." << std::endl;

	//Create an SDL Renderer
	if (!(Renderer = SDL_CreateRenderer(Window, -1, 0))) {
		std::cout << "Error: Couldn't Initialize Renderer..." << std::endl;
		return;
	}
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	std::cout << "Stage: Initialized Renderer..." << std::endl;

	IsRunning = true; //Boolean to check if window close

	//Load Background, Cat and Map textures
	Background = Texture::LoadTexture("assets/textures/background.png", Renderer);
	Catto = new Cat(Renderer, w, h);

	//Set Speed of map moving to right and pass it to map object
	int MapVelocity = w / 4.5;
	TheMap = new Map(Renderer, w, h, MapVelocity);

	SDL_Point Size; // Point to get dimensions of texture to get it's height / width ratio
	Titles[0] = Texture::LoadTexture("assets/textures/ready.png", Renderer); //Get Ready Texture
	Titles[1] = Texture::LoadTexture("assets/textures/end.png", Renderer); //Game Over Texture
	Titles[2] = Texture::LoadTexture("assets/textures/pause.png", Renderer); //Pause Button Texture
	SDL_QueryTexture(Titles[0], NULL, NULL, &Size.x, &Size.y);

	//Game Over and Get Ready Texture rectangle dimensions + positions
	TitleRect[0].w = (2 * width) / 10;
	TitleRect[1].w = (2 * width) / 10;
	TitleRect[0].x = (width - TitleRect[0].w) / 2;
	TitleRect[1].x = (width - TitleRect[1].w) / 2;

	TitleRect[0].y = (3 * height) / 4;
	TitleRect[1].y = (3 * height) / 4;
	TitleRect[0].h = TitleRect[0].w * ((float) Size.y / (float) Size.x);
	TitleRect[1].h = TitleRect[1].w * ((float) Size.y / (float) Size.x);

	//Pause button dimensions + positions
	TitleRect[2].w = width / 10;
	TitleRect[2].h = TitleRect[2].w;
	TitleRect[2].x = (width - TitleRect[2].w) / 2;
	TitleRect[2].y = (3 * height) / 4;

	//Initialize SDL Mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Error: Couldn't Initialize Mixer..." << std::endl;
		return;
	}
	std::cout << "Stage: Initialized Mixer..." << std::endl;

	//Audio and Music Objects
	Jump = new Audio("assets/audio/jump.wav", 30, 1);
	Death = new Audio("assets/audio/death.wav", 50, -1);
	Point = new Audio("assets/audio/point.wav", 20, 0);
	BGM = new Music("assets/audio/bgm.mp3", 60);
}

void Game::JumpCat() {
	//Jump function also used to start the game
	if (State == Menu) {
		BGM->PlayMusic();
		State = InGame;
	}

	//Check for GameState on Jumping.
	switch (State) {
		case InGame:
			Catto->Jump();	//Jump normally and play sfx if InGame 
			Jump->PlaySound();
			break;
		case DeathScreen:
			Catto->ResetCat();	//Reset Map and Cat if currently on GameOver screen, and change state to Menu
			TheMap->ResetMap();
			State = Menu;
			break;
	}
}

void Game::TogglePause() {
	switch (State) {
		case InGame:
			BGM->PauseMusic(); //If InGame, Pause game and music
			State = Pause;
			break;
		case Pause:
			BGM->ResumeMusic(); //Else if Game already Paused, Resume music and state back to InGame
			State = InGame;
			break;
	}
}

void Game::HandleEvents(const int& DeltaTime) {
	SDL_PollEvent(&Event); //Poll Events

	if (!DeathDelay || (DelayCount > 500)) { //Check for event only if it has been 500ms since death, hence "DeathDelay"
		switch (Event.type) {
			case SDL_QUIT:
				IsRunning = false; //Quit game loop
				break;
			case SDL_MOUSEBUTTONDOWN: //Jump the cat for all, click, space, and up arrow key
				JumpCat(); 
				break;
			case SDL_KEYDOWN:
				switch (Event.key.keysym.sym) {
					case SDLK_SPACE | SDLK_UP:
						JumpCat();
						break;
					case SDLK_ESCAPE: //Toggle Pause if ESC
						TogglePause();
						break;
				}
				break;
		}
		DeathDelay = false; // Remove the Delay condition and reset the DelayCount
		DelayCount = 0;
	}
	else {
		DelayCount += DeltaTime; //If it hasnt been 500ms, continue counting the time since last death
	}
}

void Game::Update(const int& DeltaTime) {
	if (State == InGame) {
		TheMap->Update(DeltaTime, Point); //Map is only updated/moving in InGame state
	}

	//Since the Cat is always updated except when paused:
	if (State != Pause) {
		if (Catto->Update(DeltaTime, TheMap, DeathDelay)) { //Check if cat dies, Catto->Update() returns true if cat dies.
			BGM->StopMusic();
			Death->PlaySound();
			State = DeathScreen; //Stop music and play death sound, and state to deathscreen
		}
	}
	SDL_RenderClear(Renderer); //Clear Renderer for next Render
}

void Game::Render() {
	SDL_RenderCopy(Renderer, Background, NULL, NULL); //Render the Background Texture

	//Render Map and Cat
	TheMap->Render();
	Catto->Render();

	//Check state to render ui textures
	switch (State) {
		case Menu:
			SDL_RenderCopy(Renderer, Titles[0], NULL, &TitleRect[0]); //Get Ready texture if in menu
			break;
		case Pause:
			SDL_RenderCopy(Renderer, Titles[2], NULL, &TitleRect[2]); //Pause Button texture if paused
			break;
		case DeathScreen:
			SDL_RenderCopy(Renderer, Titles[1], NULL, &TitleRect[1]); //Game Over texture if dead
			break;
	}

	SDL_RenderPresent(Renderer); 

	if (!InFocus()) {
		SDL_WaitEvent(NULL); //If Window not in focus, do not render and wait for window to come in focus.
	}
}

void Game::Clean() {
	//Free everything and clean
	free(Catto);
	free(TheMap);
	SDL_DestroyTexture(Background);
	SDL_DestroyTexture(Titles[0]);
	SDL_DestroyTexture(Titles[1]);
	SDL_DestroyTexture(Titles[2]);
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

bool Game::InFocus() {
	//Get the window's flags and check if it is In Focus.
	return (SDL_GetWindowFlags(Window) & (SDL_WINDOW_MINIMIZED | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS));
}