#include "Scene.hpp"

void Scene::tick(double deltaTime) const { for (const auto& object : objects) { object->tick(deltaTime); } }

void Scene::addObject(Object* object) { objects.emplace_back(object); }
void Scene::addLight(Light* light) { lights.emplace_back(light); }

void Scene::removeObject(Object* object) {
    erase(objects, object);
}

void Scene::removeLight(Light* light) {
    erase(lights, light);
}

Scene::~Scene() {
    for (const auto object : objects)
        delete object;
}
