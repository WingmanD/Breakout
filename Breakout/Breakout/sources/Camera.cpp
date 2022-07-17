#include "Camera.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

Camera::Camera(int* width, int* height, glm::vec3 eyeLocation, glm::vec3 center) : center(center), width(width),
    height(height) {
    setLocation(eyeLocation);

    front = center - eyeLocation;
    focalDistance = length(front);

    front = normalize(front);

    if (front == -worldUp) { right = normalize(cross(front, {0, 0, -1})); }
    else
        right = normalize(cross(front, worldUp));

    viewUp = normalize(cross(right, front));
}


void Camera::rotate(glm::vec3 rot) {
    glm::mat4 rM = glm::mat4(1.0f);
    rM = glm::rotate(rM, -rot.z, glm::vec3(0, 1, 0));
    front = glm::vec3(rM * glm::vec4(front, 1.0f));
    right = glm::vec3(rM * glm::vec4(right, 1.0f));
    viewUp = glm::vec3(rM * glm::vec4(viewUp, 1.0f));

    rM = glm::mat4(1.0f);
    rM = glm::rotate(rM, rot.y, right);
    glm::vec3 newFront = glm::vec3(rM * glm::vec4(front, 1.0f));
    glm::vec3 newViewUp = glm::vec3(rM * glm::vec4(viewUp, 1.0f));

    float angle = glm::degrees(glm::angle(newFront, glm::vec3(newFront.x, 0, newFront.z)));
    if (angle < 89.0f) {
        front = newFront;
        viewUp = newViewUp;
    }
}

glm::mat4 Camera::getViewMatrix() {
    right = normalize(cross(front, viewUp));
    viewUp = normalize(cross(right, front));

    return lookAt(getLocation(), getLocation() + front, viewUp);
}

glm::mat4 Camera::getProjectionMatrix() const {
    const float aspectRatio = static_cast<float>(*width) / static_cast<float>(*height);
    if (*width == 0 || *height == 0) return glm::mat4(1.0f);

    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::getViewUp() const { return viewUp; }
glm::vec3 Camera::getFront() const { return front; }
glm::vec3 Camera::getRight() const { return right; }
