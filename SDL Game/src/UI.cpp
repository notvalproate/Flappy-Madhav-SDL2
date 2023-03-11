#include "UI.hpp"
#include "Texture.hpp"
#include <iostream>

UIComponent::UIComponent(const SDL_Rect& Src, const SDL_Rect& Dest) {
	Selected = false;
	SrcRect = Src;
	DestRect = Dest;
}

UIComponent::~UIComponent() { }

bool UIComponent::CheckClick() {
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (
		x >= DestRect.x &&
		x <= DestRect.x + DestRect.w &&
		y >= DestRect.y &&
		y <= DestRect.y + DestRect.h
		) {
		return true;
	}

	return false;
}

void UIComponent::RenderComponent(SDL_Renderer* Ren, SDL_Texture* Tex) {
	SDL_RenderCopy(Ren, Tex, &SrcRect, &DestRect);
}

UI::UI(const char* texpath, SDL_Renderer* ren, const int& width, const int& height) {
	//Initialize state as closed and load texture
	Open = false;
	Renderer = ren;
	UITex = Texture::LoadTexture(texpath, ren);
	Shadow = Texture::LoadTexture("assets/textures/shadow.png", ren);

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

	Src.x += Src.w;
	Dest.x = ((2 * width) / 3) - Dest.w - (width / 18);

	SMode = new UIComponent(Src, Dest);
}

UI::~UI() {

}

bool UI::CheckClick() {
	if (!Open && Settings->CheckClick()) {
		Open = true;
		return true;
	}

	if (Open && Menu->CheckClick()) {
		if (NMode->CheckClick()) {
			std::cout << "Pressed NMODE";
		}
		if (SMode->CheckClick()) {
			std::cout << "Pressed SMOD";
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