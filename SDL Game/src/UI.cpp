#include "UI.hpp"
#include "Texture.hpp"
#include <iostream>
#include <cmath>
#include "GameModes.hpp"

UIComponent::UIComponent(const SDL_Rect& Src, const SDL_Rect& Dest) {
	SrcRect = Src;
	DestRect = Dest;
	Original = Dest;
	AnimLength = 120;
	KeyFrame = 0;
	State = Unselected;
}

UIComponent::~UIComponent() { }

void UIComponent::UnSelect() {
	if (State == Selected) State = AnimateOut;
}

bool UIComponent::CheckClick() {
	SDL_Point P;
	SDL_GetMouseState(&P.x, &P.y);

	if (SDL_PointInRect(&P, &DestRect)) {
		switch (State) {
		case Unselected:
			State = AnimateIn;
			return true;
		case NoSelection:
			return true;
		}
	}

	return false;
}

void UIComponent::ScaleAboutCenter(const float& factor) {
	DestRect.x = Original.x - std::round(((factor - 1) * Original.w) / (float) 2);
	DestRect.y = Original.y - std::round(((factor - 1) * Original.h) / (float) 2);
	DestRect.w = std::round(factor * Original.w);
	DestRect.h = std::round(factor * Original.h);
}

void UIComponent::Update(const int& DeltaTime) {
	if (State == AnimateIn) {
		KeyFrame += DeltaTime;
		if (KeyFrame >= AnimLength) {
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
	if (State == AnimateOut) {
		KeyFrame += DeltaTime;
		if (KeyFrame >= AnimLength) {
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
	Src = { 0, 0, 27, 28 };

	Dest.x = height / 20;
	Dest.w = width / 10;
	Dest.h = (Dest.w * 28) / 27;
	Dest.y = ((height * 19) / 20) - Dest.h;

	Settings = new UIComponent(Src, Dest);
	Settings->State = NoSelection;

	Src.x += Src.w;
	Src.w = 83;
	Src.h = 84;

	Dest.x = width / 3;
	Dest.w = width / 3;
	Dest.h = (Dest.w * 84) / 83;
	Dest.y = (height / 2) - (Dest.h) / 2;

	Menu = new UIComponent(Src, Dest);
	Menu->State = NoSelection;
	Src.x += Src.w;
	Src.w = 27;
	Src.h = 28;

	Dest.x = width / 3 + (Dest.w - width / 6) / 3;
	Dest.w = width / 12;
	Dest.h = (Dest.w * 28) / 27;
	Dest.y = (height / 2) - (Dest.h);

	NMode = new UIComponent(Src, Dest);
	NMode->State = Selected;
	NMode->ScaleAboutCenter(1.2);

	Src.x += Src.w;
	Dest.x = ((2 * width) / 3) - Dest.w - (width / 18);

	SMode = new UIComponent(Src, Dest);
}

UI::~UI() { }

bool UI::HandleEvents(const SDL_Event& event, Audio* Click, Music* BGM) {
	switch (event.type){
	case SDL_MOUSEBUTTONDOWN:
		if (!Open && Settings->CheckClick()) {
			Click->PlaySound();
			Open = true;
			return true;
		}
		
		if (Open && Menu->CheckClick()) {
			if (NMode->CheckClick()) {
				Click->PlaySound();
				SMode->UnSelect();
				TheMap->SetMode(Normal);
				TheCat->SetMode(Normal);
			}
			else if (SMode->CheckClick()) {
				Click->PlaySound();
				NMode->UnSelect();
				TheMap->SetMode(Speedy);
				TheCat->SetMode(Speedy);
			}
			else {
				if (SFXSlider->HandleEvent(event)) {
					Click->PlaySound();
				}
				if (MusicSlider->HandleEvent(event)) {
					BGM->PlayMusic();
				}
			}
			return true;
		}
		Open = false;
		return false;

	case SDL_MOUSEMOTION: case SDL_MOUSEBUTTONUP:
		if (Open) {
			if (SFXSlider->HandleEvent(event)) {
				Click->PlaySound();
			}
			if (MusicSlider->HandleEvent(event)) {
				if (event.type == SDL_MOUSEBUTTONUP) {
					BGM->StopMusic();
				}
				else {
					BGM->PlayMusic();
				}
			}
			return true;
		}
		return false;
	default:
		if(Open) return true;
		return false;
	}
}

int UI::GetSFXVol() {
	return SFXSlider->GetValue();
}

int UI::GetMusicVol() {
	return MusicSlider->GetValue();
}

void UI::Update(const int& DeltaTime) {
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