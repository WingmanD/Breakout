#include "StaticMesh.hpp"

#include <glm/gtx/vector_angle.hpp>

#include "Util.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

StaticMesh::StaticMesh(const aiMesh* mesh, Material* material): material(material) {
    indices.reserve(3 * mesh->mNumFaces);
    vertices.reserve(mesh->mNumVertices);
    triangles.reserve(mesh->mNumFaces);

    // load normals, vertex colors and texture coordinates if present
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.emplace_back(i, mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        if (mesh->HasNormals())
            vertices[i].normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                           mesh->mNormals[i].z);
        if (mesh->HasVertexColors(0))
            vertices[i].colour = glm::vec3(mesh->mColors[0][i].r, mesh->mColors[0][i].g,
                                           mesh->mColors[0][i].b);
        if (mesh->HasTextureCoords(0))
            vertices[i].uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }

    // load indices and triangles
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        Vertex* v0 = &vertices[mesh->mFaces[i].mIndices[0]];
        Vertex* v1 = &vertices[mesh->mFaces[i].mIndices[1]];
        Vertex* v2 = &vertices[mesh->mFaces[i].mIndices[2]];

        Triangle t(v0, v1, v2);
        triangles.emplace_back(t);

        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.emplace_back(mesh->mFaces[i].mIndices[j]);
    }

    indices.shrink_to_fit();


    // if no normals are present, calculate them
    if (!mesh->HasNormals()) {
        const auto normals = calculateNormals();
        for (unsigned int i = 0; i < vertices.size(); i++)
            vertices[i].normal = normals[i];
    }

    // calculate axis aligned bounding box, local space
    boundingBox = calculateBoundingBox();

    StaticMesh::init();
}

std::vector<StaticMesh*> StaticMesh::batchImport(const std::filesystem::path& path, Material* materialOverride,
                                                 Shader* shaderOverride) {
    if (!exists(path)) {
        std::cerr << "Scene::load: File does not exist: " << path << std::endl;
        return {};
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType | aiProcess_FlipUVs);

    if (!scene) {
        std::cerr << "Scene::load: Loading error: " << importer.GetErrorString() << std::endl;
        return {};
    }

    if (scene->HasMeshes()) {
        std::vector<StaticMesh*> meshes;
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            Material* material;
            if (materialOverride)
                material = materialOverride;
            else if (!shaderOverride) {
                std::cerr << "Scene::load: No material or shader specified for mesh " << i << std::endl;
                return {};
            }
            else if (scene->mMaterials[scene->mMeshes[i]->mMaterialIndex])
                material = new Material(new Shader(*shaderOverride),
                                        scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]);
            else
                material = new Material(new Shader(*shaderOverride));

            meshes.emplace_back(new StaticMesh(scene->mMeshes[i], material));
        }
        return meshes;
    }
    else {
        std::cerr << "Scene::load: No meshes found" << std::endl;
        return {};
    }

}

void StaticMesh::init() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(4, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, colour)));

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(),
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

}

void StaticMesh::draw() { draw(Transform()); }

void StaticMesh::draw(const Transform& transform) {
    material->getShader()->use();
    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(material->getShader()->ID, "model"), 1, GL_FALSE,
                       &transform.getModelMatrix()[0][0]);

    material->apply();
    material->applyTextures();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glUseProgram(0);
}

std::vector<glm::vec3> StaticMesh::calculateNormals() const {
    std::vector<glm::vec3> normals(vertices.size());

    for (const auto triangle : triangles) {
        glm::vec3 e1 = triangle.v1->position - triangle.v0->position;
        glm::vec3 e2 = triangle.v2->position - triangle.v0->position;

        glm::vec3 normal = cross(e1, e2);
        normal = normalize(normal);

        normals[triangle.v0->index] += normal;
        normals[triangle.v1->index] += normal;
        normals[triangle.v2->index] += normal;
    }
    for (auto& normal : normals) normal = normalize(normal);

    return normals;
}

BoundingBox StaticMesh::calculateBoundingBox() const {
    glm::vec3 min, max;
    min = max = vertices[0].position;

    for (const auto& vertex : vertices) {
        if (vertex.position.x < min.x) { min.x = vertex.position.x; }
        if (vertex.position.y < min.y) { min.y = vertex.position.y; }
        if (vertex.position.z < min.z) { min.z = vertex.position.z; }

        if (vertex.position.x > max.x) { max.x = vertex.position.x; }
        if (vertex.position.y > max.y) { max.y = vertex.position.y; }
        if (vertex.position.z > max.z) { max.z = vertex.position.z; }
    }

    return {min, max};
}

glm::vec3 StaticMesh::getCenter() const {
    auto center = glm::vec3(0);
    for (const auto& vertex : vertices) center += vertex.position;
    center /= vertices.size();

    return center;
}

float StaticMesh::calculateVolume() const {
    const glm::vec3 center = getCenter();
    float volume = 0;
    for (const auto triangle : triangles) volume += Util::tetrahedronVolume(triangle, center);

    return volume;
}

float StaticMesh::calculateSurfaceArea() const {
    float area = 0;
    for (const auto triangle : triangles) area += triangle.getArea();

    return area;
}
