#include "Transform.hpp"

#include <glm/gtx/quaternion.hpp>

glm::mat4 Transform::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    model = translate(model, Location);
    model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, Scale);

    if (parent) model = parent->getModelMatrix() * model;
    return model;
}

glm::mat4 Transform::getViewMatrix() const { return inverse(getModelMatrix()); }

//todo
glm::vec3 Transform::getGlobalLocation() const {
    if (parent)
        return parent->getModelMatrix() * glm::vec4(Location, 1.0f);
    return Location;
}

glm::vec3 Transform::getGlobalRotation() const {
    if (!parent) return Rotation;

    glm::quat rotationAroundParent = glm::quat(parent->Rotation);
    glm::quat localRotation = glm::quat(Rotation);
    glm::quat globalRotation = rotationAroundParent * localRotation * inverse(rotationAroundParent);

    return eulerAngles(globalRotation);
}

glm::vec3 Transform::getRelativeLocationFrom(Transform* other) const {
    return inverse(other->getModelMatrix()) * glm::vec4(getGlobalLocation(), 1.0f);
}

void Transform::attachTo(Transform* newParent) {
    if (parent) detach();

    parent = newParent;
    Location -= parent->getGlobalLocation();
}

void Transform::detach() {
    if (!parent) return;

    Location = getGlobalLocation();
    parent = nullptr;
}
