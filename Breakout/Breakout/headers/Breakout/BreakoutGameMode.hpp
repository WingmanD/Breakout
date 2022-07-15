#pragma once
#include "GameMode.hpp"

class BreakoutGameMode : public GameMode {
    int livesCount = 5;
    int score = 0;
    int level = 0;

public:
    explicit BreakoutGameMode(int lives);

    void updateScore(int points);
    void lostBall();

    void loadNextLevel();

    void start() override;
    void end() override;

    [[nodiscard]] int getLivesCount() const { return livesCount; }
    [[nodiscard]] int getScore() const { return score; }
    [[nodiscard]] int getLevel() const { return level; }
};
