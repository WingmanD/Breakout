#include "Breakout/BreakoutBall.hpp"

#include "Engine.hpp"

BreakoutBall::BreakoutBall(Engine* owner, StaticMesh* mesh) : Object(owner) {

    auto boundingBox = mesh->getBoundingBox();
    radius = (boundingBox.max.x - boundingBox.min.x) / 2.0f;

    meshComp = new StaticMeshComponent(mesh);
    meshComp->attachTo(this);


    sphereCollision = new SphereCollisionComponent(radius);
    sphereCollision->attachTo(meshComp);
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
