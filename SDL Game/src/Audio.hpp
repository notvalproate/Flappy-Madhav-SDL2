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

class Music {
public:
	Music(const char* path, int vol);
	~Music();

	void SetVolume(int vol);
	void PlayMusic();
	void StopMusic();
	void PauseMusic();
	void ResumeMusic();
private:
	Mix_Music* Song;
	int Volume;
};