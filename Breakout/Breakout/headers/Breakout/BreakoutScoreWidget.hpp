#pragma once
#include "Widget.hpp"

class BreakoutScoreWidget : public Widget {
    int* lives;
    int* level;
    int* score;

public:
    BreakoutScoreWidget(int* livesCount, int* currentLevel, int* currentScore, int* windowWidth, int* windowHeight);

    void setupUI() override;
};
