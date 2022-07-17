#define STB_IMAGE_IMPLEMENTATION

#undef main
#include <iostream>

#include "Engine.hpp"
#include "Breakout/BreakoutBoard.hpp"
#include "Breakout/BreakoutGameMode.hpp"

int main(int argc, char* argv[]) {
    // initialize engine and game mode
    Engine* engine = new Engine(std::filesystem::path(argv[0]).parent_path());
    engine->setGameMode(new BreakoutGameMode(engine, 5));

    // import background
    auto planeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/plane/plane.obj",
                                               new Material(
                                                   new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    auto backgroundObject = new Object(engine);
    if (planeImport.empty()) throw std::runtime_error("Breakout plane not found");
    StaticMesh* planeMesh = planeImport[0];
    planeMesh->getMaterial()->
               setTextureMap(
                   DIFFUSE,
                   engine->getRuntimePath() / "resources" /
                   "meshes/plane/WoodFineDark004_COL_4K.jpg");
    auto background = new StaticMeshComponent(planeMesh);
    background->attachTo(backgroundObject);
    backgroundObject->setLocation({
        0, -1, 0
    });
    backgroundObject->setScale({
        5, 1, 5
    });

    engine->getScene()->addObject(backgroundObject);
    engine->getRenderer()->setUnlit(true);

    // add camera
    auto camera = new Camera(engine->getWidthRef(), engine->getHeightRef(), {0, 1, 0}, {0, 0, 0});
    engine->getScene()->setActiveCamera(camera);
    
    engine->run();


    delete engine;

    return 0;
}
