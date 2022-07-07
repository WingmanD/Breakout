#pragma once
#include <filesystem>

#include "Shader.hpp"
#include "Singleton.hpp"

class Settings : public Singleton<Settings>{
protected: Settings() = default;

public:
    std::filesystem::path runtimePath;
    
    int width = 1280;
    int height = 720;

    double sensitivity = 0.005;
    
    Shader* defaultShader = nullptr;

    
};
