#include "Breakout/BreakoutGameMode.hpp"

#include <iostream>

void BreakoutGameMode::start() {
    
}

void BreakoutGameMode::end() {
    // todo display score and game over and level, play sound
    std::cout << "Game over" << std::endl;
}

BreakoutGameMode::BreakoutGameMode(int lives) {
    livesCount = lives;
}

void BreakoutGameMode::updateScore(int points) {
    score+= points;
    std::cout << "Score: " << score << std::endl;
}

void BreakoutGameMode::lostBall() {
    livesCount--;
    std::cout << "Lives: " << livesCount << std::endl;
    
    if(livesCount<=0) 
        end();
}

void BreakoutGameMode::loadNextLevel() {
    level++;
    livesCount++;
    std::cout << "Level: " << level << std::endl;
}


