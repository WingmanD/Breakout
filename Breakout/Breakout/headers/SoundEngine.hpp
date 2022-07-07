#pragma once
#include <filesystem>
#include "Singleton.hpp"
#include "SoundCue.hpp"


class SoundEngine : public Singleton<SoundEngine> {
    std::vector<std::shared_ptr<SoundCue>> soundCues;

protected:
    SoundEngine();
    ~SoundEngine();
public:
    std::shared_ptr<SoundCue> loadSoundCue(const std::filesystem::path& relativePath);
    bool removeSoundCue(std::shared_ptr<SoundCue> soundCue);
};
