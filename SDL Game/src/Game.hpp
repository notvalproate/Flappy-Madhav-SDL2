#pragma once
#include "SDL.h"
#include "Cat.hpp"
#include "Map.hpp"
#include "Texture.hpp"
#include "Audio.hpp"

class Game {
public:
	Game();
	~Game();

	void Init(const char* title, const char* iconpath, const int& x, const int& y, int width, int height);
	void HandleEvents();
	void Update(const int& DeltaTime);
	void Render();
	void Clean();
	bool Exit() { return IsRunning; }
	int GetDelta() { return FrameDelta; }
	
private:
	bool IsRunning;
	int FrameDelta;
	int w, h;
	SDL_Window* Window;
	SDL_Renderer* Renderer;
	SDL_DisplayMode Mode;
	SDL_Event Event;
	SDL_Texture* Background;
	SDL_Texture* Titles[2];
	SDL_Rect TitleRect[2];
	Cat* Catto;
	Map* TheMap;
	Audio *Jump, *Death, *Point;
};