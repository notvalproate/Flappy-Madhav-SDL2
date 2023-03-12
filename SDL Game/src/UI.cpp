#include "UI.hpp"
#include "Texture.hpp"
#include <iostream>
#include <cmath>
#include "GameModes.hpp"

UIComponent::UIComponent(const SDL_Rect& Src, const SDL_Rect& Dest) {
	Selected = false;
	SrcRect = Src;
	DestRect = Dest;
	Original = Dest;
}

UIComponent::~UIComponent() { }

bool UIComponent::CheckClick() {
	SDL_Point P;
	SDL_GetMouseState(&P.x, &P.y);
	
	if (SDL_PointInRect(&P, &DestRect)) {
		return true;
	}

	return false;
}

void UIComponent::ScaleAboutCenter(const float& factor) {
	DestRect.x -= std::round(((factor - 1) * DestRect.w) / (float) 2);
	DestRect.y -= std::round(((factor - 1) * DestRect.h) / (float) 2);
	DestRect.w = std::round(factor * DestRect.w);
	DestRect.h = std::round(factor * DestRect.h);
}

void UIComponent::ResetScale() {
	DestRect = Original;
}

void UIComponent::ToggleSelect() {
	if (!Selected) {
		Selected = true;
		return;
	}
	Selected = false;
}

void UIComponent::RenderComponent(SDL_Renderer* Ren, SDL_Texture* Tex) {
	SDL_RenderCopy(Ren, Tex, &SrcRect, &DestRect);
}

UI::UI(const char* texpath, const char* audiopath, SDL_Renderer* ren, const int& width, const int& height, Map* map, Cat* cat) {
	//Initialize state as closed and load texture
	Open = false;
	Renderer = ren;
	UITex = Texture::LoadTexture(texpath, ren);
	Shadow = Texture::LoadTexture("assets/textures/shadow.png", ren);
	TheMap = map;
	TheCat = cat;
	Click = new Audio(audiopath, 50, 3);

	//Initialize Components
	SDL_Rect Src, Dest;
	Src.x = 0;
	Src.y = 0;
	Src.w = 27;
	Src.h = 28;

	Dest.x = height / 20;
	Dest.w = width / 10;
	Dest.h = (Dest.w * 28) / 27;
	Dest.y = ((height * 19) / 20) - Dest.h;

	Settings = new UIComponent(Src, Dest);

	Src.x += Src.w;
	Src.w = 83;
	Src.h = 48;

	Dest.x = width / 3;
	Dest.w = width / 3;
	Dest.y = (height / 2) - (Dest.w * 6) / 20;
	Dest.h = (Dest.w * 6) / 10;

	Menu = new UIComponent(Src, Dest);

	Src.x += Src.w;
	Src.w = 27;
	Src.h = 28;

	Dest.x = width / 3 + (Dest.w - width / 6) / 3;
	Dest.w = width / 12;
	Dest.h = (Dest.w * 28) / 27;
	Dest.y = (height / 2) - (Dest.w * 3) / 20;

	NMode = new UIComponent(Src, Dest);
	NMode->ScaleAboutCenter(1.3);
	NMode->ToggleSelect();

	Src.x += Src.w;
	Dest.x = ((2 * width) / 3) - Dest.w - (width / 18);

	SMode = new UIComponent(Src, Dest);
}

UI::~UI() {

}

bool UI::CheckClick() {
	if (!Open && Settings->CheckClick()) {
		Click->PlaySound();
		Open = true;
		return true;
	}

	if (Open && Menu->CheckClick()) {
		if (!NMode->Selected && NMode->CheckClick()) {
			Click->PlaySound();
			NMode->ScaleAboutCenter(1.3);
			SMode->ResetScale();
			NMode->Selected = true;
			SMode->Selected = false;
			TheMap->SetMode(Normal);
			TheCat->SetMode(Normal);
		}
		if (!SMode->Selected && SMode->CheckClick()) {
			Click->PlaySound();
			SMode->ScaleAboutCenter(1.3);
			NMode->ResetScale();
			NMode->Selected = false;
			SMode->Selected = true;
			TheMap->SetMode(Speedy);
			TheCat->SetMode(Speedy);
		}
		return true;
	}
	else {
		Open = false;
		return false;
	}
}

void UI::RenderUI() {
	Settings->RenderComponent(Renderer, UITex);

	if (Open) {
		SDL_RenderCopy(Renderer, Shadow, NULL, NULL);
		Menu->RenderComponent(Renderer, UITex);	
		NMode->RenderComponent(Renderer, UITex);
		SMode->RenderComponent(Renderer, UITex);
		return;
	}
}