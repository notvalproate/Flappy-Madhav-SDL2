#include "Game.hpp"
#include "SDL_mixer.h"
#include <iostream>

Game::Game() : IsRunning(false), FrameDelta(16), Window(nullptr), Renderer(nullptr), DeathDelay(false), DelayCount(0), State(Menu) { }
 
Game::~Game() { }

void Game::Init(const char* title, const char* iconpath, const int& x, const int& y, int width, int height) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cout << "Error: Couldn't Initialize Subsystems..." << std::endl;
		return;
	}
	std::cout << "Stage: Initialized Subsystems..." << std::endl;

	if (SDL_GetDisplayMode(0, 0, &Mode)) {
		std::cout << "Error: Couldn't Get Display Mode...Framerate set to 60" << std::endl;
	}
	else {
		std::cout << "Stage: Display Mode Initialized..." << std::endl;
		FrameDelta = 1000 / Mode.refresh_rate;
	}

	w = width;
	h = height;

	if (!(Window = SDL_CreateWindow(title, x, y, width, height, 0))) {
		std::cout << "Error: Couldn't Initialize Window..." << std::endl;
		return;
	}
	SDL_Surface* TempSurface = IMG_Load(iconpath);
	SDL_SetWindowIcon(Window, TempSurface);
	SDL_FreeSurface(TempSurface);
	std::cout << "Stage: Initialized Window..." << std::endl;

	if (!(Renderer = SDL_CreateRenderer(Window, -1, 0))) {
		std::cout << "Error: Couldn't Initialize Renderer..." << std::endl;
		return;
	}
	SDL_SetRenderDrawColor(Renderer, 0, 0, 51, 255);
	std::cout << "Stage: Initialized Renderer..." << std::endl;

	IsRunning = true;

	Background = Texture::LoadTexture("assets/textures/background.png", Renderer);
	Catto = new Cat(Renderer, w, h);
	TheMap = new Map(Renderer, w, h);

	SDL_Point Size;
	Titles[0] = Texture::LoadTexture("assets/textures/ready.png", Renderer);
	Titles[1] = Texture::LoadTexture("assets/textures/end.png", Renderer);
	Titles[2] = Texture::LoadTexture("assets/textures/pause.png", Renderer);
	SDL_QueryTexture(Titles[0], NULL, NULL, &Size.x, &Size.y);

	TitleRect[0].w = (2 * width) / 10;
	TitleRect[1].w = (2 * width) / 10;
	TitleRect[0].x = (width - TitleRect[0].w) / 2;
	TitleRect[1].x = (width - TitleRect[1].w) / 2;

	TitleRect[0].y = (3 * height) / 4;
	TitleRect[1].y = (3 * height) / 4;
	TitleRect[0].h = TitleRect[0].w * ((float) Size.y / (float) Size.x);
	TitleRect[1].h = TitleRect[1].w * ((float) Size.y / (float) Size.x);

	TitleRect[2].w = width / 10;
	TitleRect[2].h = TitleRect[2].w;
	TitleRect[2].x = (width - TitleRect[2].w) / 2;
	TitleRect[2].y = (3 * height) / 4;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Error: Couldn't Initialize Mixer..." << std::endl;
		return;
	}
	std::cout << "Stage: Initialized Mixer..." << std::endl;
	Jump = new Audio("assets/audio/jump.wav", 30, 1);
	Death = new Audio("assets/audio/death.wav", 50, -1);
	Point = new Audio("assets/audio/point.wav", 20, 0);
	BGM = new Music("assets/audio/bgm.mp3", 60, 2);
}

void Game::JumpCat() {
	if (State == Menu) {
		BGM->PlayMusic();
		State = InGame;
	}

	switch (State) {
		case InGame:
			Catto->Jump();
			Jump->PlaySound();
			break;
		case DeathScreen:
			Catto->ResetCat();
			TheMap->ResetMap();
			State = Menu;
			break;
	}
}

void Game::TogglePause() {
	if (State == InGame) {
		BGM->PauseMusic();
		State = Pause;
	}
	else if (State == Pause) {
		BGM->ResumeMusic();
		State = InGame;
	}
}

void Game::HandleEvents(const int& DeltaTime) {
	SDL_PollEvent(&Event);

	if (!DeathDelay || (DelayCount > 500)) {
		switch (Event.type) {
			case SDL_QUIT:
				IsRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				JumpCat();
				break;
			case SDL_KEYDOWN:
				switch (Event.key.keysym.sym) {
					case SDLK_SPACE | SDLK_UP:
						JumpCat();
						break;
					case SDLK_ESCAPE:
						TogglePause();
						break;
				}
				break;
		}
		DeathDelay = false;
		DelayCount = 0;
	}
	else {
		DelayCount += DeltaTime;
	}
}

void Game::Update(const int& DeltaTime) {
	switch (State) {
		case Menu:
			Catto->Update(DeltaTime, TheMap, DeathDelay);
			break;
		case InGame:
			if (Catto->Update(DeltaTime, TheMap, DeathDelay)) {
				BGM->StopMusic();
				Death->PlaySound();
				State = DeathScreen; 
			} 
			TheMap->Update(DeltaTime, Point); 
			break;
		case DeathScreen:
			Catto->Update(DeltaTime, TheMap, DeathDelay);
			break;
	}
	SDL_RenderClear(Renderer);
}

void Game::Render() {
	SDL_RenderCopy(Renderer, Background, NULL, NULL);

	TheMap->Render();
	Catto->Render();

	switch (State) {
		case Menu:
			SDL_RenderCopy(Renderer, Titles[0], NULL, &TitleRect[0]);
			break;
		case Pause:
			SDL_RenderCopy(Renderer, Titles[2], NULL, &TitleRect[2]);
			break;
		case DeathScreen:
			SDL_RenderCopy(Renderer, Titles[1], NULL, &TitleRect[1]);
			break;
	}

	SDL_RenderPresent(Renderer);

	if (!InFocus()) {
		SDL_WaitEvent(NULL);
	}
}

void Game::Clean() {
	free(Catto);
	free(TheMap);
	SDL_DestroyTexture(Background);
	SDL_DestroyTexture(Titles[0]);
	SDL_DestroyTexture(Titles[1]);
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

bool Game::InFocus() {
	return (SDL_GetWindowFlags(Window) & (SDL_WINDOW_MINIMIZED | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS));
}