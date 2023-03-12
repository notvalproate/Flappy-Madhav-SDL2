#pragma once
#include "SDL.h"
#include "Texture.hpp"
#include "Map.hpp"
#include "Cat.hpp"
#include "Audio.hpp"

class UIComponent {
public:
	friend class UI;
private:
	UIComponent(const SDL_Rect& Src, const SDL_Rect& Dest);
	~UIComponent();

	bool CheckClick();
	void ToggleSelect();
	void RenderComponent(SDL_Renderer* Ren, SDL_Texture* Tex);
	void ScaleAboutCenter(const float& factor);
	void ResetScale();

	bool Selected;
	SDL_Rect SrcRect, DestRect, Original;
};

class UI {
public:
	UI(const char* texpath, const char* audiopath, SDL_Renderer* ren, const int& width, const int& height, Map* map, Cat* cat);
	~UI();

	bool CheckClick();
	void RenderUI();

private:
	bool Open;
	UIComponent *Settings, *Menu, *NMode, *SMode;
	SDL_Texture *UITex, *Shadow;
	SDL_Renderer* Renderer;
	Audio* Click;
	Map* TheMap;
	Cat* TheCat;
};