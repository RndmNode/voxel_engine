#include "Voxel.h"


Voxel::Voxel(float x, float y, float z, float size)
    : m_X(x), m_Y(y), m_Z(z), m_Size(size)
{
    setVertexPositions();
}

Voxel::~Voxel()
{
}

void Voxel::setVertexPositions()
{
    m_Vertices[0] = { m_X, m_Y, m_Z - m_Size };
    m_Vertices[1] = { m_X + m_Size, m_Y, m_Z - m_Size };
    m_Vertices[2] = { m_X + m_Size, m_Y + m_Size, m_Z - m_Size };
    m_Vertices[3] = { m_X, m_Y + m_Size, m_Z - m_Size };
    m_Vertices[4] = { m_X, m_Y, m_Z };
    m_Vertices[5] = { m_X + m_Size, m_Y, m_Z };
    m_Vertices[6] = { m_X + m_Size, m_Y + m_Size, m_Z };
    m_Vertices[7] = { m_X, m_Y + m_Size, m_Z };
}