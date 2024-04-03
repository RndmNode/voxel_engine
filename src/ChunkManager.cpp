#include "ChunkManager.h"
#include <iostream>

ChunkManager::ChunkManager(siv::PerlinNoise::seed_type seed)
    :   m_Mesh(new Mesh()),
        m_World_Seed(seed)
{
    GetChunksWithinRenderDistance(ChunkPosition(0, 0));
}

ChunkManager::~ChunkManager()
{
    // Clean up
    for (auto& chunk : m_Chunks)
        delete chunk.second;
    
    delete m_Mesh;
}

void ChunkManager::UpdateMesh()
{
    // Clear mesh data first before adding new chunks
    m_Mesh->m_Instances.clear();
    m_Faces = 0;

    // Build chunk meshes
    for (auto& chunk : m_Chunks)
    {
        chunk.second->BuildMesh();
    }

    // Add all chunks to mesh
    for (auto& chunk : m_Chunks)
    {
        m_Mesh->m_Instances.insert(m_Mesh->m_Instances.end(), chunk.second->m_Mesh->m_Instances.begin(), chunk.second->m_Mesh->m_Instances.end());
        m_Faces += chunk.second->m_Faces;
    }
}

void ChunkManager::GetChunksWithinRenderDistance(ChunkPosition playerPos)
{
    // Get all possible chunks within render distance
    for (int x = -RENDER_DISTANCE; x < RENDER_DISTANCE; x++)
    {
        for (int z = -RENDER_DISTANCE; z < RENDER_DISTANCE; z++)
        {
            ChunkPosition chunkPos = ChunkPosition(playerPos.x + x * CHUNK_SIZE, playerPos.z + z * CHUNK_SIZE);
            if (m_Chunks.find(chunkPos) == m_Chunks.end())
            {
                m_Chunks.insert({chunkPos, new Chunk(chunkPos, m_World_Seed, this)});
            }
        }
    }

    UpdateMesh();
}