#define STB_IMAGE_IMPLEMENTATION

#undef main
#include <iostream>

#include "Engine.hpp"
#include "Breakout/BreakoutBoard.hpp"
#include "Breakout/BreakoutGameMode.hpp"

int main(int argc, char* argv[]) {
    std::string xmlPath = (std::filesystem::path(argv[0]).parent_path() / "resources/levels/level1.xml").string();

    Engine* engine = new Engine(std::filesystem::path(argv[0]).parent_path());
    engine->setGameMode(new BreakoutGameMode(engine, 5));

    
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
                   "meshes/plane/TexturesCom_MetalThreadplate5_512_albedo.png");
    auto background = new StaticMeshComponent(planeMesh);
    background->attachTo(backgroundObject);
    backgroundObject->setLocation({
        0, -1, 0
    });
    backgroundObject->setScale({
        100, 1, 100
    });
    background->getMesh()->getMaterial()->setTextureScale({100, 100});

    engine->getScene()->addObject(backgroundObject);
    engine->getRenderer()->setUnlit(true);

    auto camera = new Camera(engine->getWidthRef(), engine->getHeightRef(), {0, 1, 0}, {0, 0, 0});
    engine->getScene()->setActiveCamera(camera);
    
    engine->run();


    delete engine;

    return 0;
}
