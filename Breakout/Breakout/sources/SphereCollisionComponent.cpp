#include "SphereCollisionComponent.hpp"

#include <glm/geometric.hpp>

#include "BoxCollisionComponent.hpp"

CollisionResult SphereCollisionComponent::collide(Collision* other) {
    if (auto otherBox = dynamic_cast<BoxCollisionComponent*>(other)) {
        glm::vec3 thisGlobalLocation = getGlobalLocation();

        glm::vec3 closestPoint = {
            std::max(otherBox->getBoundingBox().min.x,
                     std::min(thisGlobalLocation.x, otherBox->getBoundingBox().max.x)),
            std::max(otherBox->getBoundingBox().min.y,
                     std::min(thisGlobalLocation.y, otherBox->getBoundingBox().max.y)),
            std::max(otherBox->getBoundingBox().min.z, std::min(thisGlobalLocation.z, otherBox->getBoundingBox().max.z))
        };

        if (isPointInside(closestPoint)) {
            return {true, closestPoint, glm::normalize(thisGlobalLocation - closestPoint)};
        }
    }

    else if (auto otherSphere = dynamic_cast<SphereCollisionComponent*>(other)) {
        float distance = glm::distance(getGlobalLocation(), otherSphere->getGlobalLocation());
        if (distance < radius + otherSphere->radius) {
            glm::vec3 normal = glm::normalize(getGlobalLocation() - otherSphere->getGlobalLocation());
            return {true, otherSphere->getGlobalLocation() + normal * otherSphere->radius, normal};
        }
    }

    return {};

}

bool SphereCollisionComponent::isPointInside(const glm::vec3& globalPoint) {
    return length(globalPoint - getGlobalLocation()) <= radius;
}
