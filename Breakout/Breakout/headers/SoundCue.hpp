#pragma once

#include <filesystem>
#include <SDL_mixer.h>
#include <string>

class SoundCue {
    std::string name;
    Mix_Chunk* chunk;

public:
    SoundCue(std::string name, Mix_Chunk* chunk): name(std::move(name)), chunk(chunk) {}

    void play(int channel) const;
    void setVolume(int volume) const;


    ~SoundCue();
};
