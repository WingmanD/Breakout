#include "Scene.hpp"

#include <iostream>

#include "Settings.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

void Scene::tick(double deltaTime) const { for (const auto& object : objects) { object->tick(deltaTime); } }

Object* Scene::load(std::filesystem::path relativePath, Shader* shader, bool bLoadMaterials) {
    relativePath = Settings::getInstance().runtimePath / "resources" / relativePath;

    if(!exists(relativePath)) {
        std::cout << "Scene::load: File " << relativePath << " does not exist" << std::endl;
        return nullptr;
    }


    std::cout << "Loading scene: " << relativePath << std::endl;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(relativePath.string(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);

    if (!scene)
        std::cerr << importer.GetErrorString();

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        Material* material;
        if (scene->HasMaterials() && bLoadMaterials)
            material = new Material(scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);
        else
            material = new Material();

        objects.emplace_back(new Object(new StaticMesh(scene->mMeshes[i]), material, shader));
    }

    //delete scene;
    return objects.back();
}


void Scene::addObject(Object* object) { objects.emplace_back(object); }
void Scene::addLight(Light* light) { lights.emplace_back(light); }

Scene::~Scene() {
    for (const auto object : objects)
        delete object;
}
