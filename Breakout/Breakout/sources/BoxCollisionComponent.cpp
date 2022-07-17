#include "BoxCollisionComponent.hpp"

#include "SphereCollisionComponent.hpp"

BoxCollisionComponent::BoxCollisionComponent(const glm::vec3& size) {
    this->aabb.min = glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2);
    this->aabb.max = glm::vec3(size.x / 2, size.y / 2, size.z / 2);
}

CollisionResult BoxCollisionComponent::collide(Collision* other) {
    if (const auto sphere = dynamic_cast<SphereCollisionComponent*>(other)) {
        const glm::vec3 sphereToAABBLocation = toLocal(sphere->getGlobalLocation());

        // calculate closest point on AABB to sphere center
        const glm::vec3 closestPoint = {
            std::max(aabb.min.x,
                     std::min(sphereToAABBLocation.x, aabb.max.x)),
            std::max(aabb.min.y,
                     std::min(sphereToAABBLocation.y, aabb.max.y)),
            std::max(aabb.min.z,
                     std::min(sphereToAABBLocation.z, aabb.max.z))
        };
        
        float distance = glm::distance(closestPoint, sphereToAABBLocation);

        // if sphere center is inside box, distance is 0 so we need to calculate normal and point differently
        if (distance < 0.000001f) {
            const glm::vec3 normal = normalize(sphereToAABBLocation);
            return {true, toGlobal(sphereToAABBLocation), normal};
        }

        // if sphere center is outside box, return closest point
        if (distance <= sphere->getRadius()) 
            return {true, toGlobal(closestPoint), normalize(sphereToAABBLocation - closestPoint)};
        
    }

    return {};
}

bool BoxCollisionComponent::isPointInside(const glm::vec3& globalPoint) {
    glm::vec3 relativePoint = toLocal(globalPoint);
    if (aabb.max.x <= relativePoint.x && relativePoint.x <= aabb.min.x)
        if (aabb.max.y <= relativePoint.y && relativePoint.y <= aabb.min.y)
            if (aabb.max.z <= relativePoint.z && relativePoint.z <= aabb.min.z)
                return true;

    return false;
}
