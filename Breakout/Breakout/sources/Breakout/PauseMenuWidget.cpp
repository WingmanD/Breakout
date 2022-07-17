#include "Breakout/PauseMenuWidget.hpp"

PauseMenuWidget::PauseMenuWidget(GameMode* mode, int* windowWidth, int* windowHeight): Widget("Main Menu", windowWidth,
    windowHeight), gameMode(mode) {
    windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void PauseMenuWidget::setupUI() {
    auto size = ImGui::CalcTextSize("PAUSED");
    position = {(static_cast<float>(*width) - size.x) / 2, (static_cast<float>(*height) - size.y * 4) / 2};

    ImGui::Text("PAUSED");
    ImGui::Separator();
    if (ImGui::Button("RESUME")) { gameMode->resume(); }
    if (ImGui::Button("QUIT")) { gameMode->quit(); }
}
