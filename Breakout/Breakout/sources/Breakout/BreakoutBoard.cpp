#include "Breakout/BreakoutBoard.hpp"
#include "Engine.hpp"

BreakoutBoard::BreakoutBoard(Engine* owningEngine, const BreakoutLevelInfo& level): Player(owningEngine) {
    auto meshImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/brick/brick.obj",
                                              new Material(
                                                  new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (meshImport.empty()) throw std::runtime_error("BreakoutBoard: brick mesh not found");
    StaticMesh* brickMesh = meshImport[0];

    for (auto [name, type] : level.brickTypeMap) {
        brickNameSMeshMap[name] = new StaticMesh(*brickMesh);
        brickNameSMeshMap[name]->setMaterial(new Material(new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
        brickNameSMeshMap[name]->getMaterial()->setTextureMap(
            DIFFUSE, engine->getRuntimePath() / "resources" / type->texturePath);
    }

    auto brickAABB = brickMesh->getBoundingBox();
    auto brickWidth = std::max(brickAABB.max.x - brickAABB.min.x, brickAABB.max.z - brickAABB.min.z);
    auto brickHeight = std::min(brickAABB.max.x - brickAABB.min.x, brickAABB.max.z - brickAABB.min.z);

    auto planeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/plane/plane.obj",
                                               new Material(
                                                   new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (planeImport.empty()) throw std::runtime_error("BreakoutBoard: plane not found");

    StaticMesh* planeMesh = planeImport[0];
    planeMesh->getMaterial()->
               setTextureMap(DIFFUSE, engine->getRuntimePath() / "resources" / level.backgroundImagePath);

    auto background = new StaticMeshComponent(planeMesh);

    float cellWidth = brickWidth + level.columnSpacing * brickWidth * 0.1f;
    float cellHeight = brickHeight + level.rowSpacing * brickHeight * 0.1f;

    boardWidth = static_cast<float>(level.columnCount) * cellWidth;
    float boardHeight = static_cast<float>(level.rowCount) * cellHeight;

    float totalBoardHeight = 9 / 16.0f * boardWidth;

    float backgroundScale = std::max(boardWidth * 1.1f, boardHeight * 1.1f);

    //std::cout << "tex scale " << planeMesh->getMaterial()->getTextureScale().x << std::endl;
    //todo fix this
    //planeMesh->getMaterial()->setTextureScale(glm::vec2(backgroundScale));
    background->setScale(glm::vec3(backgroundScale));

    addSceneComponent(background);


    bricks.resize(level.rowCount, std::vector<Brick*>(level.columnCount));
    for (auto i = 0; i < level.rowCount; i++) {
        for (auto j = 0; j < level.columnCount; j++) {
            if (level.bricks[i][j]) {
                auto brick = new Brick(level.bricks[i][j]);
                brick->meshComp = new StaticMeshComponent(brickNameSMeshMap[brick->info->name]);

                brick->meshComp->setLocation({
                    static_cast<float>(j) * cellWidth + cellWidth / 2.0f - boardWidth / 2.0f,
                    0.1f,
                    static_cast<float>(i) * (cellHeight) + cellHeight / 2.0f
                    - totalBoardHeight / 2.0f
                });

                addSceneComponent(brick->meshComp);
                bricks[i][j] = brick;
            }
        }
    }

    auto playerMeshImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/player/player.obj",
                                                    new Material(
                                                        new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (playerMeshImport.empty()) throw std::runtime_error("BreakoutBoard: player mesh not found");
    StaticMesh* playerMesh = playerMeshImport[0];
    std::cout << "player mesh " << playerMesh->getIndices().size() << std::endl;
    playerMesh->getMaterial()->setTextureMap(
        DIFFUSE, engine->getRuntimePath() / "resources/meshes/player/MetalPipeWallRusty_albedo.png");

    player = new StaticMeshComponent(playerMesh);

    player->setLocation({
        0.0f,
        0.1f,
        totalBoardHeight / 2.0f
    });

    addSceneComponent(player);

    auto ballMeshImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/sphere/sphere.obj",
                                                  new Material(
                                                      new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (ballMeshImport.empty()) throw std::runtime_error("BreakoutBoard: ball mesh not found");

    StaticMesh* ballMesh = ballMeshImport[0];
    ballMesh->getMaterial()->setTextureMap(
        DIFFUSE, engine->getRuntimePath() / "resources/meshes/sphere/2k_moon.jpg");


    auto camera = new Camera(engine->getWidthRef(), engine->getHeightRef(), this->Location + glm::vec3(0, 2, 0),
                             this->Location);

    float cameraDistance = std::max((totalBoardHeight * 1.3f) / std::tan(glm::radians(camera->getFov())),
                                    (boardWidth * 1.3f) / (static_cast<float>(engine->getWidth()) / static_cast<float>(
                                        engine->getHeight()) * std::tan(
                                        glm::radians(camera->getFov()))));

    camera->setLocation(this->Location + glm::vec3(0, cameraDistance, 0));

    engine->getScene()->setActiveCamera(camera);

}

void BreakoutBoard::tick(double deltaTime) {}

void BreakoutBoard::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_S && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({0, 1, 0}); }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({0, -1, 0}); }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({-1, 0, 0}); }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({1, 0, 0}); }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) { engine->getScene()->getActiveCamera()->move({0, 0, 1}); }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        engine->getScene()->getActiveCamera()->move({0, 0, -1});
    }
}

void BreakoutBoard::onMouseMove(double xpos, double ypos) {
    float newX = Util::clamp(0.05f, 0.95f, static_cast<float>(xpos) / static_cast<float>(engine->getWidth())) *
        boardWidth
        - boardWidth / 2.0f;
    player->setLocation({newX, player->Location.y, player->Location.z});
}
