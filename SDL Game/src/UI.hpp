#pragma once
#include "SDL.h"
#include "Texture.hpp"

class UIComponent {
public:
	friend class UI;
private:
	UIComponent(const SDL_Rect& Src, const SDL_Rect& Dest);
	~UIComponent();

	bool CheckClick();
	void RenderComponent(SDL_Renderer* Ren, SDL_Texture* Tex);

	bool Selected;
	SDL_Rect SrcRect, DestRect;
};

class UI {
public:
	UI(const char* texpath, SDL_Renderer* ren, const int& width, const int& height);
	~UI();

	bool CheckClick();
	void RenderUI();

private:
	bool Open;
	UIComponent *Settings, *Menu, *NMode, *SMode;
	SDL_Texture *UITex, *Shadow;
	SDL_Renderer* Renderer;
};