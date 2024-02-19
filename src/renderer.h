#pragma once

#include <GL/glew.h>

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

#define ASSERT(x) if (!(x)) __builtin_debugtrap(); // kill program
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)) // check for errors on openGL calls

// Error Handling
// --------------
// clear errors
void GLClearError();

// Convert GLenum error code to string
char const* gl_error_string(GLenum const err) noexcept;

// Get errors on the function call
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private:

public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};