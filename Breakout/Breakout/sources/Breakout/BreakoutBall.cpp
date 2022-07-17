#include "Breakout/BreakoutBall.hpp"

#include "Engine.hpp"

BreakoutBall::BreakoutBall(Engine* owner, StaticMesh* mesh) : Object(owner) {

    auto boundingBox = mesh->getBoundingBox();
    radius = (boundingBox.max.x - boundingBox.min.x) / 2.0f;

    meshComp = new StaticMeshComponent(mesh);
    meshComp->attachTo(this);

    sphereCollision = new SphereCollisionComponent(radius);
    sphereCollision->attachTo(meshComp);

    light = new Light({0, 0, 0}, {1, 1, 1}, 0.5f);
    light->attachTo(this);
    light->setLocation({0, 1, 0});
    engine->getScene()->addLight(light);
}

bool BreakoutBall::collideWith(Collision* other) {
    const CollisionResult hit = sphereCollision->collide(other);
    if (hit.bCollided) {
        move(hit.collisionNormal *
            (radius - distance(getGlobalLocation(), hit.collisionPoint)));
        velocity = reflect(velocity, hit.collisionNormal);

        return true;
    }

    return false;
}

void BreakoutBall::destroy() {
    engine->getScene()->removeLight(light);
    light->destroy();
    
    Object::destroy();
}
