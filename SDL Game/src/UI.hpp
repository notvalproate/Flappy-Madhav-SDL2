#pragma once
#include "SDL.h"
#include "Texture.hpp"
#include "Map.hpp"
#include "Cat.hpp"
#include "Audio.hpp"
#include "Slider.hpp"

enum ComponentState {
	Unselected,
	AnimateIn,
	AnimateOut,
	Selected,
	NoSelection
};

class UIComponent {
public:
	friend class UI;
private:
	UIComponent(const SDL_Rect& Src, const SDL_Rect& Dest);
	~UIComponent();

	bool CheckClick();
	void RenderComponent(SDL_Renderer* Ren, SDL_Texture* Tex);
	void ScaleAboutCenter(const float& factor);
	void Update(const int& DeltaTime);
	void UnSelect();
	bool IsSelected() { if (State == Selected) return true; return false; }

	int KeyFrame, AnimLength;
	SDL_Rect SrcRect, DestRect, Original;
	ComponentState State;
};

class UI {
public:
	UI(const char* texpath, SDL_Renderer* ren, const int& width, const int& height, Map* map, Cat* cat);
	~UI();
	
	bool HandleEvents(const SDL_Event& event, Audio* Click, Music* BGM);
	void RenderUI();
	void Update(const int& DeltaTime);
	int GetSFXVol();
	int GetMusicVol();

private:
	bool Open;
	UIComponent *Settings, *Menu, *NMode, *SMode;
	Slider* SFXSlider , * MusicSlider;

	SDL_Texture *UITex, *Shadow;
	SDL_Renderer* Renderer;
	
	Map* TheMap;
	Cat* TheCat;
};