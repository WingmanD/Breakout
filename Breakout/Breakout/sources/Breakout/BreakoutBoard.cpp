#include "Breakout/BreakoutBoard.hpp"
#include "Engine.hpp"

BreakoutBoard::BreakoutBoard(Engine* owningEngine, const BreakoutLevelInfo& level): Player(owningEngine) {
    auto shader = new Shader(engine->getRuntimePath() / "shaders/blinnPhong");

    auto meshImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/brick/brick.obj", nullptr,
                                              shader);
    if (meshImport.empty()) throw std::runtime_error("BreakoutBoard: brick mesh not found");

    StaticMesh* brickMesh = meshImport[0];

    for (auto [name, type] : level.brickTypeMap) {
        brickNameSMeshMap[name] = new StaticMesh(*brickMesh);
        brickNameSMeshMap[name]->material->setTextureMap(
            DIFFUSE, engine->getRuntimePath() / "resources" / type->texturePath);
    }

    auto brickAABB = brickMesh->getBoundingBox();
    auto brickWidth = std::max(brickAABB.max.x - brickAABB.min.x, brickAABB.max.z - brickAABB.min.z);
    auto brickHeight = std::min(brickAABB.max.x - brickAABB.min.x, brickAABB.max.z - brickAABB.min.z);

    auto planeImport = StaticMesh::batchImport(engine->getRuntimePath() / "resources/meshes/plane/plane.obj", nullptr,
                                               shader);
    if (planeImport.empty()) throw std::runtime_error("BreakoutBoard: plane not found");

    StaticMesh* planeMesh = planeImport[0];
    planeMesh->material->setTextureMap(DIFFUSE, engine->getRuntimePath() / "resources" / level.backgroundImagePath);

    auto background = new StaticMeshComponent(planeMesh);

    float boardWidth = level.columnCount * (brickWidth + level.columnSpacing * brickWidth * 0.1f);
    float boardHeight = level.rowCount * (brickHeight + level.rowSpacing * brickHeight * 0.1f);

    background->setScale(glm::vec3(std::max(boardWidth, boardHeight)));
    addSceneComponent(background);


    bricks.resize(level.rowCount, std::vector<Brick*>(level.columnCount));
    for (auto i = 0; i < level.rowCount; i++) {
        for (auto j = 0; j < level.columnCount; j++) {
            if (level.bricks[i][j]) {
                auto brick = new Brick(level.bricks[i][j]);
                brick->meshComp = new StaticMeshComponent(brickNameSMeshMap[brick->info->name]);

                brick->meshComp->setLocation({
                    j * (level.columnSpacing * 0.1 * brickWidth + brickWidth) + level.columnSpacing * 0.1 *
                    brickWidth - boardWidth / 2 + brickWidth / 2,
                    0.1,
                    i * (level.rowSpacing * 0.1 * brickHeight + brickHeight) + level.rowSpacing * 0.1 * brickHeight -
                    boardHeight / 2 + brickHeight / 2
                });

                addSceneComponent(brick->meshComp);
                bricks[i][j] = brick;
            }
        }
    }

    auto camera = new Camera(engine->getWidthRef(), engine->getHeightRef(), this->Location + glm::vec3(0, 2, 0),
                             this->Location);
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
