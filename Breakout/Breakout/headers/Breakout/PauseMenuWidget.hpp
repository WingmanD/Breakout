﻿#pragma once
#include "BreakoutGameMode.hpp"
#include "Widget.hpp"

class PauseMenuWidget : public Widget {
    BreakoutGameMode* gameMode = nullptr;
public:
    PauseMenuWidget(BreakoutGameMode* mode, int* windowWidth, int* windowHeight);

    void setupUI() override;
};
