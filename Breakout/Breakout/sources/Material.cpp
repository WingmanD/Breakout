#include "Material.hpp"

#include "Shader.hpp"
#include <glad/glad.h>

Material::Material(Shader* shader, const aiMaterial* material, const std::filesystem::path& scenePath) {
    this->shader = shader;

    material->Get(AI_MATKEY_NAME, name);
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    material->Get(AI_MATKEY_SHININESS, shininess);
    material->Get(AI_MATKEY_OPACITY, opacity);

    loadTextures(material, scenePath.parent_path());
    setup();
}

Material::Material(Shader* shader) : shader(shader) { setup(); }

void Material::setup() {
    shader->use();
    apply();

    for (auto& [type, texture] : textures) setupTexture(texture);

    applyTextures();
    glUseProgram(0);
}

void Material::setTextureMap(TextureType type, const std::filesystem::path& path) {
    textures[type] = new Texture2D(path, type);
    setupTexture(textures[type]);
}

void Material::apply() {
    glUniform3fv(glGetUniformLocation(shader->ID, "ambientColor"), 1, &ambient[0]);
    glUniform3fv(glGetUniformLocation(shader->ID, "diffuseColor"), 1, &diffuse[0]);
    glUniform3fv(glGetUniformLocation(shader->ID, "specularColor"), 1, &specular[0]);
    glUniform3fv(glGetUniformLocation(shader->ID, "emissiveColor"), 1, &emissive[0]);

    glUniform1f(glGetUniformLocation(shader->ID, "shininess"), shininess);
    glUniform1f(glGetUniformLocation(shader->ID, "opacity"), opacity);

    glUniform2fv(glGetUniformLocation(shader->ID, "textureScale"), 1, value_ptr(textureScale));
    glUniform2fv(glGetUniformLocation(shader->ID, "textureOffset"), 1, value_ptr(textureOffset));
}

void Material::applyTextures() {
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[static_cast<TextureType>(i)]->ID);
    }
}

void Material::loadTextures(const aiMaterial* material, const std::filesystem::path& textureDirPath) {
    aiString texturePosition;

    for (int i = DIFFUSE; i < OPACITY; i++) {
        TextureType type = static_cast<TextureType>(i);
        if (material->GetTexture(static_cast<aiTextureType>(i + 1), 0, &texturePosition) == AI_SUCCESS) {
            std::filesystem::path texturePath = textureDirPath / texturePosition.C_Str();

            try { setTextureMap(type, texturePath); }
            catch (std::runtime_error& e) { std::cerr << e.what() << std::endl; }
        }
    }
}

void Material::setupTexture(Texture2D* texture) {
    shader->use();

    glActiveTexture(GL_TEXTURE0 + texture->type);
    glBindTexture(GL_TEXTURE_2D, texture->ID);

    glUniform1i(glGetUniformLocation(shader->ID, TextureTypeNames[texture->type].c_str()), texture->type);
    glUniform1i(glGetUniformLocation(shader->ID, std::string(TextureTypeNames[texture->type] + "Present").c_str()), 1);
    
    glUseProgram(0);
}
