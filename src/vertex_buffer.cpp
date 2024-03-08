#include "vertex_buffer.h"
#include "renderer.h"
#include "vertex_buffer_layout.h"

#include <iostream>

/*
    VertexBuffer class
    This class is used to create a vertex buffer and bind it to the shader program.
    It contains the renderer ID of the vertex buffer and functions to bind and unbind the vertex buffer.
    The vertex buffer is used to store the vertices of the objects to be rendered.
*/

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    // generate buffer obj
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}