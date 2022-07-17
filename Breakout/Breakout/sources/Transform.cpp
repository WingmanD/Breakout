#include "Transform.hpp"

#include <iostream>
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

glm::mat4 Transform::getViewMatrix() const { return glm::mat4(1); }

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

glm::vec3 Transform::getGlobalScale() const {
    if (!parent) return Scale;

    return parent->getGlobalScale() * Scale;
}

glm::vec3 Transform::toGlobal(glm::vec3 relativeLocation) const {
    return getModelMatrix() * glm::vec4(relativeLocation, 1.0f);
}

glm::vec3 Transform::toLocal(glm::vec3 globalLocation) const {
    return inverse(getModelMatrix()) * glm::vec4(globalLocation, 1.0f);
}

void Transform::attachTo(Transform* newParent) {
    if (!newParent) {
        std::cerr << "Trying to attach to nullptr" << std::endl;
        return;
    }

    if (parent) detach();

    parent = newParent;
    parent->children.emplace_back(this);

    Location -= parent->getGlobalLocation();
}

void Transform::detach() {
    if (!parent) return;

    std::erase(parent->children, this);

    Location = getGlobalLocation();
    parent = nullptr;
}

void Transform::destroy() {
    std::vector<Transform*> childrenCopy = children;
    for (auto child : childrenCopy) 
        child->destroy();
    
    detach();
}

Transform::~Transform() {
    destroy();
}
