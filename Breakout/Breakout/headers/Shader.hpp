#pragma once

#include <filesystem>
#include <string>
#include <glad/glad.h>

enum ShaderType { PROGRAM, VERTEX, FRAGMENT, GEOMETRY, COMPUTE };

const std::string ShaderTypeStrings[] = {"PROGRAM", "VERTEX", "FRAGMENT", "GEOMETRY", "COMPUTE"};

/**
 *  A class for loading and compiling shaders.
 */
class Shader {
    static void checkCompilerErrors(unsigned shader, ShaderType type);

    std::filesystem::path path;

    bool readShader(std::filesystem::path, ShaderType type, GLuint& shaderID) const;

    void init(std::filesystem::path);

public:
    unsigned int ID{};

    Shader(std::filesystem::path);

    Shader(const Shader& other) { init(other.path); }

    Shader(Shader&& other) noexcept { init(other.path); }

    Shader& operator=(const Shader& other) {
        if (this == &other)
            return *this;
        init(other.path);
        return *this;
    }

    Shader& operator=(Shader&& other) noexcept {
        if (this == &other)
            return *this;
        init(other.path);
        return *this;
    }

    ~Shader();

    void use() const;
    void setUniform(const std::string& name, bool value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
};
