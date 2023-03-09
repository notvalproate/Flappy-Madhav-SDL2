#include "Audio.hpp"

Audio::Audio(const char* path, int vol, int chan) {
	Sound = Mix_LoadWAV(path);
	Volume = (MIX_MAX_VOLUME * vol) / 100;
	Channel = chan;
}

Audio::~Audio() { 
	Mix_FreeChunk(Sound);
}

void Audio::PlaySound() {
	Mix_Volume(Channel, Volume);
	Mix_PlayChannel(Channel, Sound, 0);
}

Music::Music(const char* path, int vol) {
	Song = Mix_LoadMUS(path);
	Volume = (MIX_MAX_VOLUME * vol) / 100;
}

Music::~Music() {
	Mix_FreeMusic(Song);
}

void Music::PlayMusic() {
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}

	Mix_VolumeMusic(Volume);
	Mix_PlayMusic(Song, 10);
}

void Music::StopMusic() {
	Mix_HaltMusic();
}

void Music::PauseMusic() {
	Mix_PauseMusic();
}

void Music::ResumeMusic() {
	Mix_ResumeMusic();
}