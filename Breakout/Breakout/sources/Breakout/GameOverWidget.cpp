#include "Breakout/GameOverWidget.hpp"

GameOverWidget::GameOverWidget(BreakoutGameMode* mode, int* windowWidth, int* windowHeight): Widget("Game Over",
    windowWidth,
    windowHeight), gameMode(mode) {
    windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void GameOverWidget::setupUI() {
    auto size = ImGui::CalcTextSize("GAME OVER");
    position = {(static_cast<float>(*width) - size.x) / 2, (static_cast<float>(*height) - size.y * 4) / 2};

    ImGui::SetWindowSize({size.x * 1.5f, size.y * 10});
    
    ImGui::Text("GAME OVER");
    ImGui::Separator();
    ImGui::Text("Score: %d", gameMode->getScore());
    ImGui::Text("Level: %d", gameMode->getLevel());
    ImGui::Separator();
    if (ImGui::Button("MAIN MENU")) { gameMode->openMainMenu(); }
    if (ImGui::Button("QUIT")) { gameMode->quit(); }
}
