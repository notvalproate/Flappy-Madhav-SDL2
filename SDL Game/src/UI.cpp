#include "UI.hpp"
#include "Texture.hpp"
#include <iostream>
#include <cmath>
#include "GameModes.hpp"

UIComponent::UIComponent(const SDL_Rect& Src, const SDL_Rect& Dest) {
	//Initialize rectangles and animation length;
	SrcRect = Src;
	DestRect = Dest;
	Original = Dest;
	AnimLength = 120;
	KeyFrame = 0;
	State = Unselected;
}

UIComponent::~UIComponent() { }

void UIComponent::UnSelect() {
	//Set state to animateout and subsequently unselected
	if (State == Selected) State = AnimateOut;
}

bool UIComponent::CheckClick() {
	SDL_Point P;
	SDL_GetMouseState(&P.x, &P.y);

	//Check for click on the component
	if (SDL_PointInRect(&P, &DestRect)) {
		switch (State) {
		case Unselected:
			State = AnimateIn; //If unselected and clicked, animate in and subsequently make selected
			return true;
		case NoSelection:
			return true;  //If component is a no selection object, always return true on click
		}
	}

	//Return false, not detecting click if it is during animation or already selected
	return false;
}

void UIComponent::ScaleAboutCenter(const float& factor) {
	//Scale the rectangle about it's center, relative to its original size
	DestRect.x = Original.x - std::round(((factor - 1) * Original.w) / (float) 2);
	DestRect.y = Original.y - std::round(((factor - 1) * Original.h) / (float) 2);
	DestRect.w = std::round(factor * Original.w);
	DestRect.h = std::round(factor * Original.h);
}

void UIComponent::Update(const int& DeltaTime) {
	//Animation In logic
	if (State == AnimateIn) {
		KeyFrame += DeltaTime;
		if (KeyFrame >= AnimLength) { //Once animation is over, reset keyframe and state to selected
			KeyFrame = 0;
			State = Selected;
			return;
		}
		
		if(KeyFrame <= 50) {
			float ratio = static_cast<float>(KeyFrame) / static_cast<float>(50);
			ratio = ratio * -0.2;
			ScaleAboutCenter(1.0 + ratio);
		}
		else if (KeyFrame > 50 && KeyFrame <= 100) {
			float ratio = static_cast<float>(KeyFrame - 50) / static_cast<float>(50);
			ratio = (1.0 - ratio) * -0.2;
			ScaleAboutCenter(1.0 + ratio);
		}
		else {
			float ratio = static_cast<float>(KeyFrame - 100) / static_cast<float>(AnimLength - 100);
			ratio = ratio * 0.2;
			ScaleAboutCenter(1.0 + ratio);
		}
		return;
	}
	//Animation Out Logic
	if (State == AnimateOut) {
		KeyFrame += DeltaTime;
		if (KeyFrame >= AnimLength) { //Once animation is over, reset keyframe and state to selected
			KeyFrame = 0;
			State = Unselected;
			return;
		}
		float ratio = static_cast<float>(KeyFrame) / static_cast<float>(AnimLength);
		ratio = ratio * 0.15;
		ScaleAboutCenter(1.1 - ratio);
	}
}

void UIComponent::RenderComponent(SDL_Renderer* Ren, SDL_Texture* Tex) {
	SDL_RenderCopy(Ren, Tex, &SrcRect, &DestRect);
}



UI::UI(const char* texpath, SDL_Renderer* ren, const int& width, const int& height, Map* map, Cat* cat) {
	//Initialize state as closed and load texture
	Open = false;
	Renderer = ren;
	UITex = Texture::LoadTexture(texpath, ren);
	Shadow = Texture::LoadTexture("assets/textures/shadow.png", ren);
	TheMap = map;
	TheCat = cat;
	SFXSlider = new Slider((width / 2) - (width / 10) + (width / 30), height / 2 + height / 4.5, width / 5, width / 60, 0, 100, 50, ren);
	MusicSlider = new Slider((width / 2) - (width / 10) + (width / 30), height / 2 + height / 6.8, width / 5, width / 60, 0, 100, 50, ren);

	//Initialize Components
	SDL_Rect Src, Dest;

	//Src and position for Settings button
	Src = { 0, 0, 27, 28 };

	Dest.x = height / 20;
	Dest.w = width / 10;
	Dest.h = (Dest.w * 28) / 27;
	Dest.y = ((height * 19) / 20) - Dest.h;

	Settings = new UIComponent(Src, Dest);
	Settings->State = NoSelection; //No different states, hence no selection

	//Src and position for Menu background
	Src.x += Src.w;
	Src.w = 83;
	Src.h = 84;

	Dest.x = width / 3;
	Dest.w = width / 3;
	Dest.h = (Dest.w * 84) / 83;
	Dest.y = (height / 2) - (Dest.h) / 2;

	Menu = new UIComponent(Src, Dest);
	Menu->State = NoSelection; //No different states, hence no selection

	//Src and position for Normal Mode button
	Src.x += Src.w;
	Src.w = 27;
	Src.h = 28;

	Dest.x = width / 3 + (Dest.w - width / 6) / 3;
	Dest.w = width / 12;
	Dest.h = (Dest.w * 28) / 27;
	Dest.y = (height / 2) - (Dest.h);

	NMode = new UIComponent(Src, Dest);
	//Set to selected by default and scale it
	NMode->State = Selected;
	NMode->ScaleAboutCenter(1.2);

	//Src and position for Speedy Mode button (same as normal mode except x position changed)
	Src.x += Src.w;
	Dest.x = ((2 * width) / 3) - Dest.w - (width / 18);

	SMode = new UIComponent(Src, Dest);
}

UI::~UI() { }

bool UI::HandleClick(const SDL_Event& event, Audio* Click, Music* BGM) {
	//Check click on settings button only when menu is not open
	if (!Open && Settings->CheckClick()) {
		Click->PlaySound();
		Open = true;
		return true;
	}
	
	//Check for click on menu only when menu is open
	if (Open && Menu->CheckClick()) {
		//Check if normal mode was clicked, if yes, unselect Speedy mode and set the map and cat modes
		if (NMode->CheckClick()) {
			Click->PlaySound();
			SMode->UnSelect();
			TheMap->SetMode(Normal);
			TheCat->SetMode(Normal);
		}
		//Vice versa for click on speedy mode
		else if (SMode->CheckClick()) {
			Click->PlaySound();
			NMode->UnSelect();
			TheMap->SetMode(Speedy);
			TheCat->SetMode(Speedy);
		}
		//Else check for events on the sliders, and play respective sounds for user to check volume
		else {
			if (SFXSlider->HandleEvent(event)) {
				Click->PlaySound();
			}
			else if (MusicSlider->HandleEvent(event)) {
				BGM->PlayMusic();
			}
		}
		return true;
	}
	//If none, means click was outside the menu, so close the menu
	Open = false;
	return false;
}

bool UI::HandleMotionUp(const SDL_Event& event, Audio* Click, Music* BGM) {
	//Only check this if it is Open
	if (!Open) return false;

	//If motion/up detected on the sfx slider, play the sound
	if (SFXSlider->HandleEvent(event)) {
		Click->PlaySound();
		return true;
	}
	//If Up detected, stop the music as mouse is no longer on music slider
	if (MusicSlider->HandleEvent(event)) {
		if (event.type == SDL_MOUSEBUTTONUP) {
			BGM->StopMusic();
		}
		return true;
	}
	//Either way, return true, as it is not a click, and return false will close the menu in game.cpp but not in the UI
	//we return false only when there is a click outside the meny area
	return true;
}

bool UI::HandleEvents(const SDL_Event& event, Audio* Click, Music* BGM) {
	//Handle each event
	switch (event.type){
	case SDL_MOUSEBUTTONDOWN:
		return HandleClick(event, Click, BGM);

	case SDL_MOUSEMOTION: case SDL_MOUSEBUTTONUP:
		return HandleMotionUp(event, Click, BGM);

	default:
		if(Open) return true; //Return true by default if menu is open, if not open, return false
		return false;
	}
}

//Self Explanatory
int UI::GetSFXVol() {
	return SFXSlider->GetValue();
}

int UI::GetMusicVol() {
	return MusicSlider->GetValue();
}

void UI::SetSliders(const int& sfx, const int& music) {
	SFXSlider->SetAbsValue(sfx);
	MusicSlider->SetAbsValue(music);
}

void UI::Update(const int& DeltaTime) {
	//Update Buttons with deltatime to animate
	SMode->Update(DeltaTime);
	NMode->Update(DeltaTime);
}

void UI::RenderUI() {
	Settings->RenderComponent(Renderer, UITex);

	if (Open) {
		SDL_RenderCopy(Renderer, Shadow, NULL, NULL);
		Menu->RenderComponent(Renderer, UITex);	
		NMode->RenderComponent(Renderer, UITex);
		SMode->RenderComponent(Renderer, UITex);
		SFXSlider->Render();
		MusicSlider->Render();
		return;
	}
}