#include "Breakout/Paddle.hpp"
#include "Engine.hpp"

Paddle::Paddle(Engine* const owner, const std::filesystem::path& meshPath, const std::filesystem::path& texturePath):
    Object(owner) {
    auto paddleMeshImport = StaticMesh::batchImport(meshPath,
                                                    new Material(
                                                        new Shader(engine->getRuntimePath() / "shaders/blinnPhong")));
    if (paddleMeshImport.empty()) throw std::runtime_error("Paddle: paddle mesh not found");

    StaticMesh* paddleMesh = paddleMeshImport[0];
    paddleMesh->getMaterial()->setTextureMap(
        DIFFUSE, texturePath);

    meshComp = new StaticMeshComponent(paddleMesh);
    meshComp->attachTo(this);

    boxCollision = new BoxCollisionComponent(paddleMesh->getBoundingBox());
    boxCollision->attachTo(meshComp);


    auto boundingBox = paddleMesh->getBoundingBox();
    halfHeight = (boundingBox.max.z - boundingBox.min.z) / 2;
}
