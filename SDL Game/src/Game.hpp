#pragma once
#include "SDL.h"
#include "Cat.hpp"
#include "Map.hpp"
#include "Texture.hpp"
#include "Audio.hpp"

enum GameState {
	Menu,
	InGame,
	Pause,
	DeathScreen
};

class Game {
public:
	Game();
	~Game();

	void Init(const char* title, const char* iconpath, const int& x, const int& y, int width, int height);
	void HandleEvents(const int& DeltaTime);
	void Update(const int& DeltaTime); 
	void Render();
	void Clean();
	inline bool Exit() { return IsRunning; }
	inline int GetDelta() { return FrameDelta; }
	
private:
	GameState State;
	bool IsRunning, DeathDelay;
	int DelayCount;
	int FrameDelta;
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
	Audio *Jump, *Death, *Point;
	Music *BGM;

	bool InFocus();
	void JumpCat();
	void TogglePause();
};