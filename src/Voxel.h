#pragma once

#include <array>

#include "vendor/glm/glm.hpp"

// struct Vertex 
// {
//     glm::vec3 Position;
//     glm::vec4 Color;
//     glm::vec2 TexCoords;
//     float TexIndex;
// };

// extern unsigned int const voxel_indices[36] = {
//     0, 1, 2, 2, 3, 0, // front
//     4, 5, 6, 6, 7, 4, // back
//     0, 4, 7, 7, 3, 0, // left
//     1, 5, 6, 6, 2, 1, // right
//     0, 1, 5, 5, 4, 0, // top
//     3, 2, 6, 6, 7, 3  // bottom
// };

class Voxel
{
public:
    Voxel(float x, float y, float z, float size = 1.0f);
    ~Voxel();
    float getX() { return m_X; }
    float getY() { return m_Y; }
    float getZ() { return m_Z; }
    float getSize() { return m_Size; }
    void setX(float x) { m_X = x; }
    void setY(float y) { m_Y = y; }
    void setZ(float z) { m_Z = z; }
    void setSize(float size) { m_Size = size; }

    void setVertexPositions();

    std::array<glm::vec3, 8> m_Vertices;
    float m_Indices[36];
private:
    float m_X, m_Y, m_Z;
    float m_Size;
};