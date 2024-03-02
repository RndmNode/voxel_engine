#pragma once

#include <string>
#include <unordered_map>

#include "vendor/glm/glm.hpp"

// Shader Source Struct
struct ShaderProgramSource 
{
    std::string VertexSource;
    std::string FragmentSource;
};

/**
 * @class Shader
 * @brief Represents a shader program used for rendering.
 * 
 * The Shader class encapsulates the functionality of a shader program,
 * including loading, compiling, and linking shaders, as well as setting
 * uniforms and binding/unbinding the shader for rendering.
 */
class Shader 
{
private:
    std::string m_filepath;
    unsigned int m_RendererID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // set uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void SetUniform1iv(const std::string& name, unsigned int count, int *values);
private:
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    int GetUniformLocation(const std::string& name) const;
};
