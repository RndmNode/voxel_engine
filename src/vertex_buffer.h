#pragma once

/*
    VertexBuffer class
    This class is used to create a vertex buffer and bind it to the shader program.
    It contains the renderer ID of the vertex buffer and functions to bind and unbind the vertex buffer.
    The vertex buffer is used to store the vertices of the objects to be rendered.
*/

class VertexBuffer
{
    private:
        unsigned int m_RendererID;
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
}; // class VertexBuffer