#define STB_IMAGE_IMPLEMENTATION

#undef main
#include <iostream>

#include "Engine.hpp"
#include "Breakout/BreakoutBoard.hpp"
#include "Breakout/BreakoutGameMode.hpp"

int main(int argc, char* argv[]) {
    std::string xmlPath = (std::filesystem::path(argv[0]).parent_path() / "resources/levels/level1.xml").string();

    Engine* engine = new Engine(std::filesystem::path(argv[0]).parent_path());
    engine->setGameMode(new BreakoutGameMode(5));
    engine->getGameMode()->start();
    
    BreakoutBoard* board = new BreakoutBoard(engine, BreakoutLevelInfo(xmlPath));
    engine->getScene()->addObject(board);

    
    engine->getScene()->addLight(new Light(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.5f));
    engine->possess(board);

    engine->run();
    

    delete engine;

    return 0;
}
