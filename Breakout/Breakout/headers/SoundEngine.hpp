#pragma once
#include <filesystem>
#include "SoundCue.hpp"

/**
 *  Engine subsystem that manages sound effects.
 */
class SoundEngine {
    std::vector<std::shared_ptr<SoundCue>> soundCues;
    
public:
    SoundEngine();
    
    std::shared_ptr<SoundCue> loadSoundCue(const std::filesystem::path& path);
    bool removeSoundCue(std::shared_ptr<SoundCue> soundCue);

    ~SoundEngine();
};
