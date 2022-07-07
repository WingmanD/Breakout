#pragma once

#include <filesystem>
#include <string>
#include <glad/glad.h>

enum ShaderType { PROGRAM, VERTEX, FRAGMENT, GEOMETRY, COMPUTE };
const std::string ShaderTypeStrings[] = {"PROGRAM", "VERTEX", "FRAGMENT", "GEOMETRY", "COMPUTE"};

class Shader {
    static void checkCompilerErrors(unsigned shader, ShaderType type);

    bool readShader(std::filesystem::path path, ShaderType type, GLuint& shaderID) const;

public:
    unsigned int ID;

    Shader(std::filesystem::path path);

    Shader(const Shader& other) = default;

    Shader(Shader&& other) noexcept
        : ID(other.ID) {}

    Shader& operator=(const Shader& other) {
        if (this == &other)
            return *this;
        ID = other.ID;
        return *this;
    }

    Shader& operator=(Shader&& other) noexcept {
        if (this == &other)
            return *this;
        ID = other.ID;
        return *this;
    }

    ~Shader();

    void use() const;
    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
};
