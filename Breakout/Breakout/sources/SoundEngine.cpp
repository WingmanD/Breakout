#include "SoundEngine.hpp"

#include <iostream>
#include <memory>
#include <SDL.h>

SoundEngine::SoundEngine() {
    // init SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;

    Mix_AllocateChannels(32);
}

SoundEngine::~SoundEngine() { SDL_Quit(); }

std::shared_ptr<SoundCue> SoundEngine::loadSoundCue(const std::filesystem::path& path) {
    Mix_Chunk* chunk = Mix_LoadWAV(path.string().c_str());

    if (!chunk) {
        std::cerr << "Failed to load sound file: " << path << std::endl;
        return nullptr;
    }

    Mix_VolumeChunk(chunk, MIX_MAX_VOLUME);

    auto cue = std::make_shared<SoundCue>(path.string(), chunk);

    soundCues.emplace_back(cue);

    return cue;
}

bool SoundEngine::removeSoundCue(std::shared_ptr<SoundCue> soundCue) {
    auto res = std::ranges::remove(soundCues, soundCue);
    soundCues.erase(res.begin(), res.end());

    return !res.empty();
}
