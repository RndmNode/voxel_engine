#include "vertex_array.h"
#include "renderer.h"
#include "vertex_buffer_layout.h"
#include <iostream>

VertexArray::VertexArray()
{
    // generate array obj
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID))
}

/*
    AddBuffer function
    This function is used to add a buffer to the vertex array.
    It takes a vertex buffer and a vertex buffer layout as parameters.
    It binds the vertex array and the vertex buffer and sets the vertex attribute pointers.
    The vertex attribute pointers are used to define the layout of the vertex buffer.
    The layout of the vertex buffer is defined by pushing elements to the layout using the Push function.
    The layout elements are used in the vertex buffer to define the position, color, and texture coordinates of the vertices
        or anything else stored in the vertex buffer.
*/

// Add buffer to vertex array
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}