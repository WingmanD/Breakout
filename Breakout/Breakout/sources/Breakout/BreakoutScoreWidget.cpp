#include "Breakout/BreakoutScoreWidget.hpp"

#include <iostream>

BreakoutScoreWidget::BreakoutScoreWidget(int* livesCount, int* currentLevel, int* currentScore, int* windowWidth,
                                         int* windowHeight) :
    Widget("BreakoutScoreWidget", windowWidth, windowHeight),
    lives(livesCount), level(currentLevel), score(currentScore) {
    windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void BreakoutScoreWidget::setupUI() {

    ImGui::Text("BALLS: %d", *lives);

    std::string scoreText = "SCORE: " + std::to_string(*score);
    ImVec2 scoreTextSize = ImGui::CalcTextSize(scoreText.c_str());

    ImGui::SetWindowSize({static_cast<float>(*width), scoreTextSize.y});
    
    ImGui::SameLine(static_cast<float>(*width) / 2.0f - scoreTextSize.x / 2);
    ImGui::Text("SCORE: %d", *score);

    std::string levelText = "LEVEL: " + std::to_string(*level);
    ImVec2 levelTextSize = ImGui::CalcTextSize(levelText.c_str());
    ImGui::SameLine(static_cast<float>(*width) - levelTextSize.x * 1.5f);
    ImGui::Text("LEVEL: %d", *level);


}
