#include "Renderer.hpp"

#include "glm/mat4x4.hpp"
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(Scene* scene, Camera* camera): camera(camera), scene(scene) {
    initViewData();
}

void Renderer::render() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateViewData();

    for (const auto& object : scene->objects) object->drawFirstPass(camera, scene->lights);
    for (const auto& object : scene->objects) object->drawSecondPass(camera, scene->lights);
}

inline void Renderer::initViewData() {
    glGenBuffers(1, &UBO_ViewData);

    glBindBuffer(GL_UNIFORM_BUFFER, UBO_ViewData);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO_ViewData, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));
}


inline void Renderer::updateViewData() const {
    glBindBuffer(GL_UNIFORM_BUFFER, UBO_ViewData);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), value_ptr(camera->getViewMatrix()));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), value_ptr(camera->getProjectionMatrix()));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), value_ptr(camera->Location));

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


Renderer::~Renderer() {
    glDeleteBuffers(1, &UBO_ViewData);
}
