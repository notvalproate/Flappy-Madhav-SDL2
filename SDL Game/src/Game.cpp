#include "Game.hpp"
#include "SDL_mixer.h"
#include <iostream>
#include <fstream>

Game::Game() : IsRunning(false), FrameDelta(16), Window(nullptr), Renderer(nullptr), DeathDelay(false), DelayCount(0), State(ReadyScreen) { }
 
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
		FrameDelta = (float) 1000 / (float) Mode.refresh_rate;
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
	if (!(Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED))) {
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
	TheMap = new Map(Renderer, w, h, w / 4.5, w / 2);

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
	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		std::cout << "Error: Couldn't Initialize Mixer..." << std::endl;
		return;
	}
	std::cout << "Stage: Initialized Mixer..." << std::endl;

	//Audio and Music Objects and get their volume
	Jump = new Audio("assets/audio/jump.wav", 50, 1);
	Death = new Audio("assets/audio/death.wav", 50, -1);
	Point = new Audio("assets/audio/point.wav", 50, 0);
	BGM = new Music("assets/audio/bgm.mp3", 50);
	Click = new Audio("assets/audio/option.wav", 50, 3);

	//Menu UI
	MenuScreen = new UI("assets/textures/ui.png", Renderer, w, h, TheMap, Catto);

	ReadVolume();
}

void Game::TogglePause() {
	switch (State) {
	case InGame:
		State = Pause;
		BGM->PauseMusic();
		break;
	case Pause:
		State = InGame; //Else if Game already Paused, Resume music and state back to InGame
		BGM->ResumeMusic();
		break;
	}
}

void Game::GameEvents(const SDL_Event& Event) {
	//Handle game window events like fullscreen, pause, and sdl_quit
	if (Event.type == SDL_QUIT) {
		WriteVolume();
		IsRunning = false;
		return;
	}
	if (Event.type == SDL_KEYDOWN) {
		switch (Event.key.keysym.sym) { //Check for which key, pause if esc, toggle fullscreen if f11
		case SDLK_ESCAPE:
			TogglePause();
			break;
		case SDLK_F11:
			//If the window doesnt have fullscreen flag, set the window to fullscreen
			std::cout << SDL_GetWindowGrab(Window) << std::endl;
			if (!(SDL_GetWindowFlags(Window) & SDL_WINDOW_FULLSCREEN)) {
				SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN); 
				SDL_SetWindowSize(Window, Mode.w, Mode.h);
				break;
			}
			//Else remove window fullscreen flag and set resolution
			SDL_SetWindowFullscreen(Window, 0); 
			SDL_SetWindowSize(Window, 1280, 720); 
			break;
		}
	}
}

void Game::HandleEvents() {
	SDL_PollEvent(&Event); //Poll Events
	if (!DeathDelay || (DelayCount > 500)) { //Check for event only if it has been 500ms since death, hence "DeathDelay"
		switch (State) {
		case Menu:
			//If menu UI was not clicked, return state to readyscreen
			if (!MenuScreen->HandleEvents(Event, Click, BGM)) {
				State = ReadyScreen;
			}
			else {
				int MusicVol = MenuScreen->GetMusicVol(); //Else get the volumes and setvolume for all
				int SFXVol = MenuScreen->GetSFXVol();
				Death->SetVolume(SFXVol);
				Point->SetVolume(SFXVol);
				Jump->SetVolume(SFXVol);
				Click->SetVolume(SFXVol);
				BGM->SetVolume(MusicVol);
			}
			break;
		case ReadyScreen:
			//If settings button was pressed, go to menu
			if (MenuScreen->HandleEvents(Event, Click, BGM)) {
				State = Menu;
			}
			//else check if cat was pressed
			else if(Catto->HandleEvents(Event)) {
				Jump->PlaySound();
				BGM->PlayMusic();
				State = InGame;
			}
			break;
		case InGame:
			//Check for click on cat and play sound
			if(Catto->HandleEvents(Event)) Jump->PlaySound();
			break;
		case DeathScreen:
			//If cat pressed on death screen, reset the map and set state to readyscreen
			if (Catto->HandleEvents(Event)) {
				State = ReadyScreen;
				TheMap->ResetMap();
			}
			break;
		}
		//Check for game events like f11 and pause at the end
		GameEvents(Event);
		DeathDelay = false; // Remove the Delay condition and reset the DelayCount
		DelayCount = 0;
	}
	else {
		DelayCount += FrameDelta; //If it hasnt been 500ms, continue counting the time since last death
	}
}

void Game::Update() {
	if (State == InGame) {
		TheMap->Update(FrameDelta, Point); //Map is only updated/moving in InGame state
	}

	if (State == Menu) {
		MenuScreen->Update(FrameDelta);
	}

	//Since the Cat is always updated except when paused:
	if (State != Pause) {
		if (Catto->Update(FrameDelta, TheMap, DeathDelay)) { //Check if cat dies, Catto->Update() returns true if cat dies.
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
		case ReadyScreen:
			SDL_RenderCopy(Renderer, Titles[0], NULL, &TitleRect[0]); //Get Ready texture if Ready
			MenuScreen->RenderUI();
			break;
		case Menu:
			MenuScreen->RenderUI(); //Render the Menu
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
		BGM->PauseMusic();
		SDL_WaitEvent(NULL); //If Window not in focus, do not render and wait for window to come in focus.
		BGM->ResumeMusic();
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

void Game::ReadVolume() {
	//Open hs.dat to read
	int sfx, music;
	std::ifstream os("assets/hs.dat", std::ios::binary);

	//If mode is speedy, offset and then read
	os.seekg(16, std::ios::beg);
	os.read((char*)&sfx, sizeof(sfx));
	os.read((char*)&music, sizeof(music));
	os.close();

	Death->SetVolume(sfx);
	Point->SetVolume(sfx);
	Jump->SetVolume(sfx);
	Click->SetVolume(sfx);
	BGM->SetVolume(music);

	MenuScreen->SetSliders(sfx, music);
}

void Game::WriteVolume() {
	int sfx = MenuScreen->GetSFXVol(), music = MenuScreen->GetMusicVol();
	std::ofstream os("assets/hs.dat", std::ios::binary | std::ios::in | std::ios::out);

	//If mode is speedy, offset and then write
	os.seekp(16, std::ios::beg);
	os.write((char*)&sfx, sizeof(sfx));
	os.write((char*)&music, sizeof(music));
	os.close();
}