#include "SoundCue.hpp"

void SoundCue::play(int channel) const {
    Mix_PlayChannel(channel, chunk, 0);
}
void SoundCue::setVolume(int volume) const {
    Mix_VolumeChunk(chunk, volume);
}
SoundCue::~SoundCue() {
    Mix_FreeChunk(chunk);
}
