#pragma once

// IndexBuffer class
//  - Represents an index buffer
//  - Stores the index data for the vertex buffer
//  - Binds and unbinds the index buffer
//  - Deletes the index buffer

class IndexBuffer
{
    private:
        unsigned int m_RendererID;
        unsigned int m_Count;

    public:
        IndexBuffer(const unsigned int* data, unsigned int size);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() const { return m_Count; }
}; // class IndexBuffer