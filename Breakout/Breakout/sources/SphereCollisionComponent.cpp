#include "SphereCollisionComponent.hpp"

#include <glm/geometric.hpp>

#include "BoxCollisionComponent.hpp"

CollisionResult SphereCollisionComponent::collide(Collision* other) {
    if (const auto otherBox = dynamic_cast<BoxCollisionComponent*>(other)) { return otherBox->collide(this); }

    if (const auto otherSphere = dynamic_cast<SphereCollisionComponent*>(other)) {
        float distance = glm::distance(getGlobalLocation(), otherSphere->getGlobalLocation());
        if (distance < radius + otherSphere->radius) {
            glm::vec3 normal = normalize(getGlobalLocation() - otherSphere->getGlobalLocation());
            return {true, otherSphere->getGlobalLocation() + normal * otherSphere->radius, normal};
        }
    }

    return {};

}

bool SphereCollisionComponent::isPointInside(const glm::vec3& globalPoint) {
    return length(globalPoint - getGlobalLocation()) <= radius;
}
