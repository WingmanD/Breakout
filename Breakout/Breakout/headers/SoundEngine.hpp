﻿#pragma once
#include <filesystem>
#include "SoundCue.hpp"


class SoundEngine {
    std::vector<std::shared_ptr<SoundCue>> soundCues;
    
public:
    SoundEngine();
    
    std::shared_ptr<SoundCue> loadSoundCue(const std::filesystem::path& relativePath);
    bool removeSoundCue(std::shared_ptr<SoundCue> soundCue);

    ~SoundEngine();
};
