#include "shader.h"
#include "renderer.h"
#include "GL/glew.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

Shader::Shader(const std::string& filepath)
    : m_filepath (filepath), m_RendererID(0)
{
    // grab shaders from file, create shaders
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

// Grab and parse shader code
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    if (!stream.is_open())
        std::cout << "ERROR: Could not open shader file" << std::endl;

    enum class ShaderType 
    {
        None = -1, VERTEX = 0, FRAGMENT = 1 
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::None;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        } 
        else 
        {
            ss[(int)type] << line << "\n";
        }
    }

    stream.close();
    return {ss[0].str(), ss[1].str()};
}

// Compile OpenGL Shader
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    // Create and compile shader
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Check for errors
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
            " Shader:" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

// Create OpenGL Shader
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Create and compile vertex and fragment shaders
    unsigned int program = glCreateProgram();
    GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    // Attach and link shaders
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    // Check for errors
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        std::cout << "Error on linking shader program" << std::endl;
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }

    // Delete shaders to free memory
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

/* 
 * Uniforms
 * 
 * Uniforms are sent to the shader program using the glUniform* functions
 * These functions are used to set the value of a uniform variable in the shader program
 * Examples of uniform variables include the model, view, and projection matrices,
 * as well as the color or texture of an object
*/

// Set Uniforms
void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1iv(const std::string& name, unsigned int count, int *values)
{
    GLCall(glUniform1iv(GetUniformLocation(name), count, values));
}

// We need to get the location of the uniform variable in the shader program in order to set its value
int Shader::GetUniformLocation(const std::string& name) const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) 
        std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
    // std::cout << "Location int: " << location << std::endl;
    m_UniformLocationCache[name] = location;

    return location;
}
