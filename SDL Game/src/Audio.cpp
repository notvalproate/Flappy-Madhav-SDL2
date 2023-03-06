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

void Audio::SetVolume(int vol) {
	Volume = (MIX_MAX_VOLUME * vol) / 100;
}