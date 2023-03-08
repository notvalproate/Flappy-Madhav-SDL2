#pragma once
#include "SDL_mixer.h"

class Audio {
public:
	Audio(const char* path, int vol, int chan);
	~Audio();

	void PlaySound();
private:
	Mix_Chunk* Sound;
	int Volume, Channel;
};

class Music {
public:
	Music(const char* path, int vol, int chan);
	~Music();

	void PlayMusic();
	void StopMusic();
	void PauseMusic();
	void ResumeMusic();
private:
	Mix_Music* Song;
	int Volume, Channel;
};