#include "Voxel.h"


// Voxel::Voxel(float x, float y, float z, float size)
//     : m_X(x), m_Y(y), m_Z(z), m_Size(size)
// {
//     // setVertexPositions();

/*
    0, 1, 2, 2, 3, 0, // front
    4, 5, 6, 6, 7, 4, // back
    0, 4, 7, 7, 3, 0, // left
    1, 5, 6, 6, 2, 1, // right
    0, 1, 5, 5, 4, 0, // top
    3, 2, 6, 6, 7, 3  // bottom

    glm::vec3 vertices[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    }
*/


//     float vertices[40] = { 
//         m_X + (m_Size / 2), m_Y + (m_Size / 2), m_Z + (m_Size / 2),     0.0f, 0.0f,
//         m_X - (m_Size / 2), m_Y + (m_Size / 2), m_Z - (m_Size / 2),     0.0f, 1.0f,
//         m_X - (m_Size / 2), m_Y + (m_Size / 2), m_Z + (m_Size / 2),     1.0f, 0.0f,
//         m_X + (m_Size / 2), m_Y - (m_Size / 2), m_Z - (m_Size / 2),     1.0f, 1.0f,
//         m_X - (m_Size / 2), m_Y - (m_Size / 2), m_Z - (m_Size / 2),     0.0f, 0.0f,
//         m_X + (m_Size / 2), m_Y + (m_Size / 2), m_Z - (m_Size / 2),     1.0f, 0.0f,
//         m_X + (m_Size / 2), m_Y - (m_Size / 2), m_Z + (m_Size / 2),     0.0f, 1.0f,
//         m_X - (m_Size / 2), m_Y - (m_Size / 2), m_Z + (m_Size / 2),     1.0f, 1.0f 
//     };

//     memcpy(m_Vertices, vertices, sizeof(float) * 40);
// }

// Voxel::~Voxel()
// {
// }

// void Voxel::setVertexPositions()
// {
    
// }