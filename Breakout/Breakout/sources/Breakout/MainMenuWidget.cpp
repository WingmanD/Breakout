#include "Breakout/MainMenuWidget.hpp"

MainMenuWidget::MainMenuWidget(GameMode* mode, int* windowWidth,
                               int* windowHeight) : Widget("Main Menu", windowWidth, windowHeight), gameMode(mode) {
    windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void MainMenuWidget::setupUI() {
    auto size = ImGui::CalcTextSize("BREAKOUT");
    position = {(static_cast<float>(*width) - size.x) / 2, (static_cast<float>(*height) - size.y * 4) / 2};
    
    ImGui::Text("BREAKOUT");
    ImGui::Separator();
    if (ImGui::Button("START")) { gameMode->start(); }
    if (ImGui::Button("QUIT")) { gameMode->quit(); }
}
