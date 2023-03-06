#pragma once
#include "SDL_mixer.h"

class Audio {
public:
	Audio(const char* path, int vol, int chan);
	~Audio();

	void PlaySound();
	void SetVolume(int vol);
private:
	Mix_Chunk* Sound;
	int Volume, Channel;
};