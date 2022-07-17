#include "Breakout/WonWidget.hpp"

WonWidget::WonWidget(BreakoutGameMode* mode, int* windowWidth, int* windowHeight): Widget("Game Over",
    windowWidth,
    windowHeight), gameMode(mode) {
    windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void WonWidget::setupUI() {
    auto size = ImGui::CalcTextSize("");
    position = {(static_cast<float>(*width) - size.x) / 2, (static_cast<float>(*height) - size.y * 4) / 2};

    ImGui::Text("YOU WIN!");
    ImGui::Separator();
    ImGui::Text("Score: %d", gameMode->getScore());
    ImGui::Separator();
    if (ImGui::Button("MAIN MENU")) { gameMode->openMainMenu(); }
    if (ImGui::Button("QUIT")) { gameMode->quit(); }
}
