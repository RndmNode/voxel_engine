#include "ChunkManager.h"

#include <iostream>

ChunkManager::ChunkManager(siv::PerlinNoise::seed_type seed)
    :   m_Mesh(new Mesh()),
        m_World_Seed(seed)
{
}

ChunkManager::~ChunkManager()
{
    // Clean up
    for (auto& chunk : m_Chunks)
        delete chunk.second;
    
    delete m_Mesh;
}

bool ChunkManager::Update()
{
    // Update player's chunk position
    m_CurrentPlayerChunkPos = ChunkPosition((int)m_PlayerPosition.x / CHUNK_SIZE, (int)m_PlayerPosition.z / CHUNK_SIZE);

    // Check if player has moved to a new chunk
    if (m_CurrentPlayerChunkPos != m_PreviousPlayerChunkPos)
    {
        std::cout << "Player has moved to a new chunk\t" << m_CurrentPlayerChunkPos.x * CHUNK_SIZE << ", " << m_CurrentPlayerChunkPos.z * CHUNK_SIZE << "\n";
        // Update player's previous chunk position
        m_PreviousPlayerChunkPos = m_CurrentPlayerChunkPos;

        // Load new chunks in render distance
        return UpdateChunksAroundPlayer();
    }
    return false;
}

bool ChunkManager::UpdateMesh()
{
    // Clear mesh data first before adding new chunks
    m_Mesh->m_Instances.clear();  /////////// THIS IS STILL RESULTING IN HAVING TO REBUILD THE ENTIRE MESH INSTEAD OF JUST ADDING NEW CHUNKS
    m_Faces = 0;

    // Build chunk meshes
    for (auto& chunk : m_Chunks)
    {
        std::cout << "Building mesh for chunk\t" << chunk.first.x << ", " << chunk.first.z << "\n";
        chunk.second->BuildMesh();
    }

    // Add all chunks to mesh
    for (auto& chunk : m_Chunks)
    {
        m_Mesh->m_Instances.insert(m_Mesh->m_Instances.end(), chunk.second->m_Mesh->m_Instances.begin(), chunk.second->m_Mesh->m_Instances.end());
        m_Faces += chunk.second->m_Faces;
    }

    return true;
}

bool ChunkManager::UpdateChunksAroundPlayer()
{
    ChunkPosition playerPos = {m_CurrentPlayerChunkPos.x * CHUNK_SIZE, m_CurrentPlayerChunkPos.z * CHUNK_SIZE};
    // Get all possible chunks within render distance
    for (int x = -RENDER_DISTANCE; x < RENDER_DISTANCE; x++)
    {
        for (int z = -RENDER_DISTANCE; z < RENDER_DISTANCE; z++)
        {
            ChunkPosition chunkPos = ChunkPosition(playerPos.x + (x * CHUNK_SIZE), playerPos.z + (z * CHUNK_SIZE));
            if (m_Chunks.find(chunkPos) == m_Chunks.end())
            {
                // Load chunk
                std::cout << "Loading chunk\t" << chunkPos.x << ", " << chunkPos.z << "\n";
                m_Chunks[chunkPos] = new Chunk(chunkPos, m_World_Seed, this);
            }
        }
    }

    // Unload chunks out of render distance + 1
    for (auto it = m_Chunks.begin(); it != m_Chunks.end();)
    {
        bool x_test, z_test;
        x_test = true ? it->first.x < playerPos.x - ((RENDER_DISTANCE + 1) * CHUNK_SIZE) || it->first.x > playerPos.x + ((RENDER_DISTANCE + 1) * CHUNK_SIZE) : false;
        z_test = true ? it->first.z < playerPos.z - ((RENDER_DISTANCE + 1) * CHUNK_SIZE) || it->first.z > playerPos.z + ((RENDER_DISTANCE + 1) * CHUNK_SIZE) : false;

        if(x_test || z_test)
        {
            delete it->second;
            it = m_Chunks.erase(it);
        }
        else
            it++;
    }

    return UpdateMesh();
}