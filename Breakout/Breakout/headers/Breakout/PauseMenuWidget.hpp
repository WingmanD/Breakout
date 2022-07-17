#pragma once
#include "GameMode.hpp"
#include "Widget.hpp"

class PauseMenuWidget : public Widget{
    GameMode* gameMode = nullptr;
public:
    PauseMenuWidget(GameMode* mode, int* windowWidth, int* windowHeight);
    
    void setupUI() override;
};
