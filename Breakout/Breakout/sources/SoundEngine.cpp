﻿#include "SoundEngine.hpp"

#include <iostream>
#include <memory>
#include <SDL.h>

#include "Settings.hpp"

SoundEngine::SoundEngine() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;

    Mix_AllocateChannels(32);
}

SoundEngine::~SoundEngine() { SDL_Quit(); }

std::shared_ptr<SoundCue> SoundEngine::loadSoundCue(const std::filesystem::path& relativePath) {
    const std::string path = (Settings::getInstance().runtimePath / "resources" / relativePath).string();

    auto chunk = Mix_LoadWAV(path.c_str());

    if (!chunk) {
        std::cerr << "Failed to load sound file: " << path << std::endl;
        return nullptr;
    }
    
    Mix_VolumeChunk(chunk, MIX_MAX_VOLUME);
    
    return std::make_shared<SoundCue>(relativePath.string(), chunk);
}

bool SoundEngine::removeSoundCue(std::shared_ptr<SoundCue> soundCue) {
    auto res = std::ranges::remove(soundCues, soundCue);
    soundCues.erase(res.begin(), res.end());
    
    return !res.empty();
}