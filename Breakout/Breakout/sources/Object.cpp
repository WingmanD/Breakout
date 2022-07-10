#include "Object.hpp"

void Object::drawFirstPass(Camera* camera, Transform instanceTransform, std::vector<Light*>& lights) {
    glUseProgram(shader->ID);

    mesh->applyTransform(camera, instanceTransform, shader);
    
    material->applyTextures();

    glBindVertexArray(mesh->getVAO());
    glDrawElements(GL_TRIANGLES, mesh->getIndices().size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Object::attachToObject(Object* object) {
    attachTo(object);
    object->childObjects.emplace_back(this);
}
