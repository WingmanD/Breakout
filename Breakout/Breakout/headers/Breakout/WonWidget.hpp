#pragma once
#include "BreakoutGameMode.hpp"
#include "Widget.hpp"

class WonWidget : public Widget{
    BreakoutGameMode* gameMode = nullptr;
public:
    WonWidget(BreakoutGameMode* mode, int* windowWidth, int* windowHeight);
    
    void setupUI() override;
};
