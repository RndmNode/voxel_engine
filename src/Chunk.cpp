#include "Chunk.h"

#include <iostream>

Chunk::Chunk(glm::ivec3 position)
    :   m_Mesh(new Mesh()),
        m_Position(position)
{
    // Set all voxels to solid
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                m_Voxels[i][j][k] = new Voxel::VoxelData({Voxel::SOLID});
            }
        }
    }
    BuildMesh();
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
    for (int i = 0; i < CHUNK_VOLUME; i++)
    {
        // Get voxel position
        int x = i % CHUNK_SIZE;
        int y = i / (CHUNK_SIZE * CHUNK_SIZE);
        int z = (i / CHUNK_SIZE) % CHUNK_SIZE;

        // Get voxel type
        if (m_Voxels[x][y][z]->m_Type == Voxel::AIR)
        {
            continue;
        }

        // Get neighbors
        NeighborList neighbors = GetNeighbors(x, y, z);
        for (auto neighbor : neighbors)
        {
            // Get neighbor face and type
            Voxel::VoxelFace face = neighbor.m_Face;
            Voxel::VoxelType type = neighbor.m_Type;

            if (type == Voxel::AIR)
            {
                // Increment face count
                m_Faces++;

                // Add instance offset to vertices
                m_Mesh->m_Instances.push_back(glm::vec4(x + m_Position.x, y + m_Position.y, z + m_Position.z, float(face)));
            }
        }
    }
}

NeighborList Chunk::GetNeighbors(int x, int y, int z)
{
    NeighborList neighbors;

    // Add neighbors that are out of bounds
    if (x <= 0)                  neighbors.push_back({Voxel::VoxelFace::LEFT,    Voxel::VoxelType::AIR});
    if (x >= CHUNK_SIZE - 1)     neighbors.push_back({Voxel::VoxelFace::RIGHT,   Voxel::VoxelType::AIR});
    if (y <= 0)                  neighbors.push_back({Voxel::VoxelFace::BOTTOM,  Voxel::VoxelType::AIR});
    if (y >= CHUNK_SIZE - 1)     neighbors.push_back({Voxel::VoxelFace::TOP,     Voxel::VoxelType::AIR});
    if (z <= 0)                  neighbors.push_back({Voxel::VoxelFace::BACK,    Voxel::VoxelType::AIR});
    if (z >= CHUNK_SIZE - 1)     neighbors.push_back({Voxel::VoxelFace::FRONT,   Voxel::VoxelType::AIR});

    // Add neighbors that are in bounds
    if (x > 0)                   neighbors.push_back({Voxel::VoxelFace::LEFT,    m_Voxels[x - 1][y][z]->m_Type});
    if (x < CHUNK_SIZE - 1)      neighbors.push_back({Voxel::VoxelFace::RIGHT,   m_Voxels[x + 1][y][z]->m_Type});
    if (y > 0)                   neighbors.push_back({Voxel::VoxelFace::BOTTOM,  m_Voxels[x][y - 1][z]->m_Type});
    if (y < CHUNK_SIZE - 1)      neighbors.push_back({Voxel::VoxelFace::TOP,     m_Voxels[x][y + 1][z]->m_Type});
    if (z > 0)                   neighbors.push_back({Voxel::VoxelFace::BACK,    m_Voxels[x][y][z - 1]->m_Type});
    if (z < CHUNK_SIZE - 1)      neighbors.push_back({Voxel::VoxelFace::FRONT,   m_Voxels[x][y][z + 1]->m_Type});

    return neighbors;
}
