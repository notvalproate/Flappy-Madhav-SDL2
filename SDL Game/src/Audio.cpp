#include "Audio.hpp"

Audio::Audio(const char* path, int vol, int chan) {
	//Load Wav into the Chunk and set volume and channel
	Sound = Mix_LoadWAV(path);
	Volume = (MIX_MAX_VOLUME * vol) / 100;
	Channel = chan;
}

Audio::~Audio() { 
	//Free the pointer
	Mix_FreeChunk(Sound);
}

void Audio::SetVolume(int vol) {
	Volume = vol;
}

void Audio::PlaySound() {
	//Set Volume of the channel and play
	Mix_Volume(Channel, Volume);
	Mix_PlayChannel(Channel, Sound, 0);
}

Music::Music(const char* path, int vol) {
	//Load MP3 into the Music object and set music channel volume
	Song = Mix_LoadMUS(path);
	Volume = (MIX_MAX_VOLUME * vol) / 100;
}

Music::~Music() {
	//Free the pointer
	Mix_FreeMusic(Song);
}

void Music::SetVolume(int vol) {
	Volume = vol;
	Mix_VolumeMusic(Volume);
}

void Music::PlayMusic() {
	//If another music object is playing, halt it and play this one.
	if (Mix_PlayingMusic()) {
		return;
	}

	Mix_VolumeMusic(Volume);
	Mix_PlayMusic(Song, 10);
}

//Self-Explanatory functions

void Music::StopMusic() {
	Mix_HaltMusic();
}

void Music::PauseMusic() {
	Mix_PauseMusic();
}

void Music::ResumeMusic() {
	Mix_ResumeMusic();
}