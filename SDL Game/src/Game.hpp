#pragma once
#include "SDL.h"
#include "Cat.hpp"
#include "Map.hpp"
#include "Texture.hpp"
#include "Audio.hpp"
#include "UI.hpp"

enum GameState {
	Menu,
	ReadyScreen,
	InGame,
	Pause,
	DeathScreen
};

class Game {
public:
	Game();
	~Game();

	void Init(const char* title, const char* iconpath, const int& x, const int& y, int width, int height);
	void HandleEvents();
	void Update(); 
	void Render();
	void GameEvents(const SDL_Event& Event);
	void Clean();
	inline bool Exit() { return IsRunning; }
	
private:
	GameState State;
	bool IsRunning, DeathDelay;
	int DelayCount;
	float FrameDelta;
	int w, h;
	SDL_Window* Window;
	SDL_Renderer* Renderer;
	SDL_DisplayMode Mode;
	SDL_Event Event;
	SDL_Texture* Background;
	SDL_Texture* Titles[3];
	SDL_Rect TitleRect[3];
	Cat* Catto;
	Map* TheMap;
	Audio *Jump, *Death, *Point, *Click;
	Music *BGM;
	UI *MenuScreen;

	bool InFocus();
	void TogglePause();
	void ReadVolume();
	void WriteVolume();
};