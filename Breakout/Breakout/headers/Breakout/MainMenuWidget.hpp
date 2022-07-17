#pragma once
#include "GameMode.hpp"
#include "Widget.hpp"

class MainMenuWidget : public Widget{
    GameMode* gameMode = nullptr;
public:
    MainMenuWidget(GameMode* mode, int* windowWidth, int* windowHeight);
    
    void setupUI() override;
};
