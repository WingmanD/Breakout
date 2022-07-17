#pragma once
#include "BreakoutGameMode.hpp"
#include "Widget.hpp"

class GameOverWidget : public Widget{
    BreakoutGameMode* gameMode = nullptr;
public:
    GameOverWidget(BreakoutGameMode* mode, int* windowWidth, int* windowHeight);
    
    void setupUI() override;
};
