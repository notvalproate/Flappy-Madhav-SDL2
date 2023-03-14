#pragma once
#include "SDL.h"
#include <algorithm>
#include "Texture.hpp"

class Slider {
public:
    Slider(int x, int y, int width, int height, int minValue, int maxValue, int startValue, SDL_Renderer* renderer);
    ~Slider();

    void Render();
    bool HandleEvent(const SDL_Event& event);
    int GetValue() { return Value; }
    void SetValue(int newValue);

private:
    int MinValue, MaxValue, Value;
    SDL_Texture* HandleTex;
    SDL_Renderer* Renderer;
    SDL_Rect HandleRect, BarRect;
    bool IsDragging;
};