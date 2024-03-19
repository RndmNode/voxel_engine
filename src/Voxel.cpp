#include "Voxel.h"


Voxel::Voxel(float x, float y, float z, float size)
    : m_X(x), m_Y(y), m_Z(z), m_Size(size)
{
    // setVertexPositions();
    float vertices[40] = { 
        m_X + (m_Size / 2), m_Y + (m_Size / 2), m_Z + (m_Size / 2),     0.0f, 0.0f,
        m_X - (m_Size / 2), m_Y + (m_Size / 2), m_Z - (m_Size / 2),     0.0f, 1.0f,
        m_X - (m_Size / 2), m_Y + (m_Size / 2), m_Z + (m_Size / 2),     1.0f, 0.0f,
        m_X + (m_Size / 2), m_Y - (m_Size / 2), m_Z - (m_Size / 2),     1.0f, 1.0f,
        m_X - (m_Size / 2), m_Y - (m_Size / 2), m_Z - (m_Size / 2),     0.0f, 0.0f,
        m_X + (m_Size / 2), m_Y + (m_Size / 2), m_Z - (m_Size / 2),     1.0f, 0.0f,
        m_X + (m_Size / 2), m_Y - (m_Size / 2), m_Z + (m_Size / 2),     0.0f, 1.0f,
        m_X - (m_Size / 2), m_Y - (m_Size / 2), m_Z + (m_Size / 2),     1.0f, 1.0f 
    };

    memcpy(m_Vertices, vertices, sizeof(float) * 40);
}

Voxel::~Voxel()
{
}

// void Voxel::setVertexPositions()
// {
    
// }