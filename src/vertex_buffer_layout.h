#pragma once

#include <vector>
// #include <GL/glew.h>

#include "renderer.h"

/*
    VertexBufferLayoutElement struct
    This struct is used to define the layout of the vertex buffer.
    It contains the type of the element, the count of the element, and whether or not it is normalized.
    It also contains a static function to get the size of the type of the element.
*/

struct VertexBufferLayoutElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    };
};

/*
    VertexBufferLayout class
    This class is used to define the layout of the vertex buffer.
    It contains a vector of VertexBufferLayoutElements and the stride of the layout.
    It also contains a template function to push elements to the layout.
    The layout of the vertex buffer is defined by pushing elements to the layout using the Push function.
    The layout elements are used in the vertex buffer to define the position, color, and texture coordinates of the vertices
        or anything else stored in the vertex buffer.
*/

class VertexBufferLayout
{
private:
    std::vector<VertexBufferLayoutElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout()
        : m_Stride(0) {};
    ~VertexBufferLayout(){};

    template<typename T>
    void Push(unsigned int count)
    {
        ASSERT("Unkown type! Please specify the type");
    }
    
    template<>
    void Push<float>(unsigned int count)
    {
        m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
        m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        m_Stride += count * VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline unsigned int GetStride() const { return m_Stride; }
    inline const std::vector<VertexBufferLayoutElement>& GetElements() const { return m_Elements; }

};