#pragma once

#include "vertex_buffer.h"

class VertexBufferLayout;

/*
    VertexArray class
    This class is used to create a vertex array and bind it to the shader program.
    It contains the renderer ID of the vertex array and functions to bind and unbind the vertex array.
    The vertex array is used to store the vertex buffer and the layout of the vertex buffer.
*/

class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;
};