#pragma once

#define VOXEL_SIZE 1.0f

#include <array>

#include "vendor/glm/glm.hpp"

// struct Vertex 
// {
//     glm::vec3 Position;
//     glm::vec4 Color;
//     glm::vec2 TexCoords;
//     float TexIndex;
// };

enum VoxelFace
{
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

enum VoxelType
{
    AIR,
    SOLID
};

struct Voxel 
{
    bool m_Active = true;
    glm::vec3 m_Position;
    glm::vec4 m_Color;
    VoxelType m_Type;
};

// class Voxel
// {
// public:
//     Voxel(float x=0.0f, float y=0.0f, float z=0.0f, float size=1.0f);
//     ~Voxel();
//     float getX() { return m_X; }
//     float getY() { return m_Y; }
//     float getZ() { return m_Z; }
//     float getSize() { return m_Size; }
//     void setX(float x) { m_X = x; }
//     void setY(float y) { m_Y = y; }
//     void setZ(float z) { m_Z = z; }
//     void setSize(float size) { m_Size = size; }
//     void setActive(bool active) { m_Active = active; }

//     // void setVertexPositions();
//     bool m_Active = true;
//     float m_Vertices[40];
//     // float m_Indices[36];
// private:
//     float m_X, m_Y, m_Z;
//     float m_Size;
    
// };