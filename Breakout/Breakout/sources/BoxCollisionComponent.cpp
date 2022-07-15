#include "BoxCollisionComponent.hpp"

#include "SphereCollisionComponent.hpp"

BoxCollisionComponent::BoxCollisionComponent(const glm::vec3& size) {
    this->aabb.min = glm::vec3(-size.x / 2, -size.y / 2, -size.z / 2);
    this->aabb.max = glm::vec3(size.x / 2, size.y / 2, size.z / 2);
}

CollisionResult BoxCollisionComponent::collide(Collision* other) {
    if (const auto sphere = dynamic_cast<SphereCollisionComponent*>(other)) {
        const glm::vec3 sphereToAABBLocation = toLocal(sphere->getGlobalLocation());


        //todo this works when ball is outside of box, but not when it is inside.
        const glm::vec3 closestPoint = {
            std::max(aabb.min.x,
                     std::min(sphereToAABBLocation.x, aabb.max.x)),
            std::max(aabb.min.y,
                     std::min(sphereToAABBLocation.y, aabb.max.y)),
            std::max(aabb.min.z,
                     std::min(sphereToAABBLocation.z, aabb.max.z))
        };

        /*std::cout << "closestPoint: " << closestPoint.x << " " << closestPoint.y << " " << closestPoint.z << std::endl;
        std::cout << "sphereToAABBLocation: " << sphereToAABBLocation.x << " " << sphereToAABBLocation.y << " " <<
            sphereToAABBLocation.z << std::endl;

        std::cout << "distance between closestPoint and sphereToAABBLocation: " << glm::distance(
            closestPoint, sphereToAABBLocation) << std::endl;*/

        float distance = glm::distance(closestPoint, sphereToAABBLocation);

        // if sphere center is inside box, distance is 0 so we need to calculate normal and point differently
        if (distance < 0.000001f) {
            const glm::vec3 normal = normalize(sphereToAABBLocation);
            return {true, toGlobal(sphereToAABBLocation), normal};
        }

        const glm::vec3 closestPointInside = {
            std::max(aabb.min.x,
                     std::min(sphereToAABBLocation.x, aabb.max.x)),
            std::max(aabb.min.y,
                     std::min(sphereToAABBLocation.y, aabb.max.y)),
            std::max(aabb.min.z,
                     std::min(sphereToAABBLocation.z, aabb.max.z))
        };

        // sphere is overlapping or touching the box
        if (distance <= sphere->getRadius()) {
            /*std::cout << "closestPoint: " << closestPoint.x << " " << closestPoint.y << " " << closestPoint.z <<
                std::endl;
            std::cout << "sphereToAABBLocation: " << sphereToAABBLocation.x << " " << sphereToAABBLocation.y << " " <<
                sphereToAABBLocation.z << std::endl;

            std::cout << "distance between closestPoint and sphereToAABBLocation: " << distance(
                closestPoint, sphereToAABBLocation) << std::endl;*/
            return {true, toGlobal(closestPoint), normalize(sphereToAABBLocation - closestPoint)};
        }
    }

    return {};
}

bool BoxCollisionComponent::isPointInside(const glm::vec3& point) {
    if (aabb.max.x <= point.x && point.x <= aabb.min.x)
        if (aabb.max.y <= point.y && point.y <= aabb.min.y)
            if (aabb.max.z <= point.z && point.z <= aabb.min.z)
                return true;

    return false;
}
