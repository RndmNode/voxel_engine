#include "Chunk.h"

#include <cstdint>
#include <vector>
#include <array>

Chunk::Chunk()
{
    // Set all voxels to solid
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                m_Voxels[i][j][k].m_Type = Voxel::VoxelType::SOLID;
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
        if (m_Voxels[x][y][z].m_Type == Voxel::VoxelType::AIR)
        {
            continue;
        }

        // Get neighbors
        NeighborList neighbors = GetNeighbors(x, y, z);
        for (auto neighbor : neighbors)
        {
            Voxel::VoxelFace face = std::get<0>(neighbor);
            Voxel::VoxelType type = std::get<1>(neighbor);
            if (type == Voxel::VoxelType::AIR)
            {
                // Increment face count
                m_Faces++;

                // Add instance offset to vertices
                m_Mesh.m_Instances.push_back(glm::vec4(x, y, z, float(face)));
            }
        }
    }
}

NeighborList Chunk::GetNeighbors(uint8_t x, uint8_t y, uint8_t z)
{
    NeighborList neighbors;

    // Add neighbors that are out of bounds
    if (x <= 0)                  neighbors.push_back(std::make_tuple(Voxel::VoxelFace::LEFT,    Voxel::VoxelType::AIR));
    if (x >= CHUNK_SIZE - 1)     neighbors.push_back(std::make_tuple(Voxel::VoxelFace::RIGHT,   Voxel::VoxelType::AIR));
    if (y <= 0)                  neighbors.push_back(std::make_tuple(Voxel::VoxelFace::BOTTOM,  Voxel::VoxelType::AIR));
    if (y >= CHUNK_SIZE - 1)     neighbors.push_back(std::make_tuple(Voxel::VoxelFace::TOP,     Voxel::VoxelType::AIR));
    if (z <= 0)                  neighbors.push_back(std::make_tuple(Voxel::VoxelFace::BACK,    Voxel::VoxelType::AIR));
    if (z >= CHUNK_SIZE - 1)     neighbors.push_back(std::make_tuple(Voxel::VoxelFace::FRONT,   Voxel::VoxelType::AIR));

    // Add neighbors that are in bounds
    if (x > 0)                   neighbors.push_back(std::make_tuple(Voxel::VoxelFace::LEFT,    m_Voxels[x - 1][y][z].m_Type));
    if (x < CHUNK_SIZE - 1)      neighbors.push_back(std::make_tuple(Voxel::VoxelFace::RIGHT,   m_Voxels[x + 1][y][z].m_Type));
    if (y > 0)                   neighbors.push_back(std::make_tuple(Voxel::VoxelFace::BOTTOM,  m_Voxels[x][y - 1][z].m_Type));
    if (y < CHUNK_SIZE - 1)      neighbors.push_back(std::make_tuple(Voxel::VoxelFace::TOP,     m_Voxels[x][y + 1][z].m_Type));
    if (z > 0)                   neighbors.push_back(std::make_tuple(Voxel::VoxelFace::BACK,    m_Voxels[x][y][z - 1].m_Type));
    if (z < CHUNK_SIZE - 1)      neighbors.push_back(std::make_tuple(Voxel::VoxelFace::FRONT,   m_Voxels[x][y][z + 1].m_Type));

    return neighbors;
}
