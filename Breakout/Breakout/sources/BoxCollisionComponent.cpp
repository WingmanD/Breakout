#include "BoxCollisionComponent.hpp"

#include "SphereCollisionComponent.hpp"

CollisionResult BoxCollisionComponent::collide(Collision* other) {
    if (const auto sphere = dynamic_cast<SphereCollisionComponent*>(other)) 
        return sphere->collide(this);

    return {};
}

bool BoxCollisionComponent::isPointInside(const glm::vec3& point) {
    if (aabb.max.x <= point.x && point.x <= aabb.min.x)
        if (aabb.max.y <= point.y && point.y <= aabb.min.y)
            if (aabb.max.z <= point.z && point.z <= aabb.min.z)
                return true;

    return false;
}
