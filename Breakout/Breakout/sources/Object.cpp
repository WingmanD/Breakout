#include "Object.hpp"
void Object::draw() {
    for (auto sceneComp : childSceneComponents) {
        sceneComp->draw();
    }
}
