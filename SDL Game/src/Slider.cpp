#include "Slider.hpp"
#include <iostream>

Slider::Slider(int x, int y, int width, int height, int minValue, int maxValue, int startValue, SDL_Renderer* renderer)
    : MinValue(minValue), MaxValue(maxValue), Value(startValue), IsDragging(false) {
    HandleTex = Texture::LoadTexture("assets/textures/handle.png", renderer);
    Renderer = renderer;

    HandleRect = { x, y, height, height };
    BarRect = { x, y + height / 3, width,  height / 3 };
}

Slider::~Slider() { }

bool Slider::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        SDL_Point MouseLoc;
        MouseLoc.x = event.button.x;
        MouseLoc.y = event.button.y;
        
        SDL_Rect Wrapper = { BarRect.x - HandleRect.w / 2, HandleRect.y, BarRect.w + HandleRect.w, HandleRect.h};

        if (SDL_PointInRect(&MouseLoc, &Wrapper)) {
            // Clicked on the slider, move the handle to the clicked position
            SetValue(MouseLoc.x);
            IsDragging = true; // Set dragging flag
            return true;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP && IsDragging) {
        // Release dragging flag
        IsDragging = false;
        return true;
    }
    else if (event.type == SDL_MOUSEMOTION && IsDragging) {
        // Dragging the handle, set value based on mouse position
        SetValue(event.motion.x);
        return true;
    }
    return false;
}


void Slider::Render() {
    SDL_SetRenderDrawColor(Renderer, 193, 168, 64, 255);
    SDL_RenderFillRect(Renderer, &BarRect);

    HandleRect.x = BarRect.x + std::round((int)(((float)(Value - MinValue) / (float)(MaxValue - MinValue)) * (float)(BarRect.w)) - (HandleRect.h / 2));
    SDL_RenderCopy(Renderer, HandleTex, NULL, &HandleRect);
}

void Slider::SetValue(int MouseX) {
    float valueRatio = (float)(MouseX - BarRect.x) / (float)BarRect.w;
    int newValue = MinValue + (int)(valueRatio * (float)(MaxValue - MinValue));
    Value = std::max(MinValue, std::min(MaxValue, newValue)); //Clamp the value between the min and max
}