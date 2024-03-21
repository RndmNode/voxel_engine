#include "Chunk.h"

#include <cstdint>
#include <vector>
#include <array>

const std::array<float, 12> FRONT_FACE_VERTICES = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

const std::array<float, 12> BACK_FACE_VERTICES = {
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f
};

const std::array<float, 12> LEFT_FACE_VERTICES = {
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f
};

const std::array<float, 12> RIGHT_FACE_VERTICES = {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f
};

const std::array<float, 12> TOP_FACE_VERTICES = {
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f
};

const std::array<float, 12> BOTTOM_FACE_VERTICES = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f
};

const float FACE_NORMALS[18] = {
    0.0f, 0.0f, -1.0f, // front
    0.0f, 0.0f,  1.0f,  // back
    -1.0f, 0.0f, 0.0f, // left
    1.0f, 0.0f,  0.0f,  // right
    0.0f, 1.0f,  0.0f,  // top
    0.0f, -1.0f, 0.0f  // bottom
};

Chunk::Chunk()
    : m_FACE_VERTICES {
        {FRONT,   FRONT_FACE_VERTICES},
        {BACK,    BACK_FACE_VERTICES},
        {LEFT,    LEFT_FACE_VERTICES},
        {RIGHT,   RIGHT_FACE_VERTICES},
        {TOP,     TOP_FACE_VERTICES},
        {BOTTOM,  BOTTOM_FACE_VERTICES}}
{
    // Set all voxels to solid
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                m_Voxels[i][j][k].m_Type = SOLID;
            }
        }

        BuildMesh();
    }
}

Chunk::~Chunk()
{
}

void Chunk::OnRender()
{
    
}

void Chunk::OnUpdate()
{
    
}

void Chunk::BuildMesh()
{
    for (uint16_t i = 0; i < CHUNK_VOLUME; i++)
    {
        // Get voxel position
        uint8_t x = i % CHUNK_SIZE;
        uint8_t y = i / (CHUNK_SIZE * CHUNK_SIZE);
        uint8_t z = (i / CHUNK_SIZE) % CHUNK_SIZE;

        // Get voxel type
        if (m_Voxels[x][y][z].m_Type == AIR)
        {
            continue;
        }

        // Get neighbors
        NeighborList neighbors = GetNeighbors(x, y, z);
        for (auto neighbor : neighbors)
        {
            VoxelFace face = std::get<0>(neighbor);
            VoxelType type = std::get<1>(neighbor);
            if (type == AIR)
            {
                // Increment face count
                m_Faces++;

                // Get base vertices for face
                std::array<float, 12> vertices = m_FACE_VERTICES.at(face);

                // Add vertices to mesh
                for (int i = 0; i < 4; i++)
                {
                    m_Mesh.m_Vertices.push_back(vertices[i * 3] + x);
                    m_Mesh.m_Vertices.push_back(vertices[i * 3 + 1] + y);
                    m_Mesh.m_Vertices.push_back(vertices[i * 3 + 2] + z);
                }
                
                // Add indices to mesh
                m_Mesh.m_Indices.insert(m_Mesh.m_Indices.end(),
                                       {m_Indicies_count, m_Indicies_count + 1, m_Indicies_count + 2,
                                        m_Indicies_count + 2, m_Indicies_count + 3, m_Indicies_count});
                m_Indicies_count += 6;
            }
        }
    }
}

NeighborList Chunk::GetNeighbors(uint8_t x, uint8_t y, uint8_t z)
{
    NeighborList neighbors;

    // Add neighbors that are out of bounds
    if (x <= 0)                  neighbors.push_back(std::make_tuple(LEFT,    VoxelType::AIR));
    if (x >= CHUNK_SIZE - 1)     neighbors.push_back(std::make_tuple(RIGHT,   VoxelType::AIR));
    if (y <= 0)                  neighbors.push_back(std::make_tuple(BOTTOM,  VoxelType::AIR));
    if (y >= CHUNK_SIZE - 1)     neighbors.push_back(std::make_tuple(TOP,     VoxelType::AIR));
    if (z <= 0)                  neighbors.push_back(std::make_tuple(BACK,    VoxelType::AIR));
    if (z >= CHUNK_SIZE - 1)     neighbors.push_back(std::make_tuple(FRONT,   VoxelType::AIR));

    // Add neighbors that are in bounds
    if (x > 0)                   neighbors.push_back(std::make_tuple(LEFT,    m_Voxels[x - 1][y][z].m_Type));
    if (x < CHUNK_SIZE - 1)      neighbors.push_back(std::make_tuple(RIGHT,   m_Voxels[x + 1][y][z].m_Type));
    if (y > 0)                   neighbors.push_back(std::make_tuple(BOTTOM,  m_Voxels[x][y - 1][z].m_Type));
    if (y < CHUNK_SIZE - 1)      neighbors.push_back(std::make_tuple(TOP,     m_Voxels[x][y + 1][z].m_Type));
    if (z > 0)                   neighbors.push_back(std::make_tuple(BACK,    m_Voxels[x][y][z - 1].m_Type));
    if (z < CHUNK_SIZE - 1)      neighbors.push_back(std::make_tuple(FRONT,   m_Voxels[x][y][z + 1].m_Type));


    return neighbors;
}
