#include "ChunkManager.h"

#include <iostream>
#include <thread>

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

void ChunkManager::UpdateChunkMeshes(std::vector<ChunkPosition> stack)
{   
    std::vector<std::thread> threads;

    // Update chunk meshes
    for (auto chunkPos : stack)
        threads.push_back(std::thread(&Chunk::BuildMesh, m_Chunks[chunkPos]));

    // Join threads
    for (auto& thread : threads)
        thread.join();

    // Updated Face count
    for (auto chunkPos : stack)
        m_Faces += m_Chunks[chunkPos]->m_Faces;
}

void ChunkManager::LoadChunk(ChunkPosition chunkPos)
{
    m_Chunks[chunkPos] = new Chunk(chunkPos, m_World_Seed, this);
    m_Chunks[chunkPos]->BuildMesh();
    m_Faces += m_Chunks[chunkPos]->m_Faces;
}

// void ChunkManager::UnloadChunk(ChunkPosition chunkPos)
// {
//     std::cout << "Unloading chunk\t" << chunkPos.x << ", " << chunkPos.z << "\n";
//     m_Faces -= m_Chunks[chunkPos]->m_Faces;
//     std::cout << "Deleting chunk...\n";
//     delete m_Chunks[chunkPos];
//     std::cout << "Erasing chunk from m_Chunks...\n";
//     m_Chunks.erase(chunkPos);
// }

bool ChunkManager::UpdateChunksAroundPlayer()
{
    std::vector<std::thread> threads;

    // Stack to keep track of chunks that need to be updated
    std::vector<ChunkPosition> ChunkStack;

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
                // LoadChunk(chunkPos);
                // std::cout << "Loading chunk\t" << chunkPos.x << ", " << chunkPos.z << "\n";
                ChunkStack.push_back(chunkPos);
                threads.push_back(std::thread(&ChunkManager::LoadChunk, this, chunkPos));
            }
        }
    }

    // Join threads
    for (auto& thread : threads)
        thread.join();

    // Update chunk meshes for new chunks
    // UpdateChunkMeshes(ChunkStack);

    // Unload chunks out of render distance + 1
    for (auto it = m_Chunks.begin(); it != m_Chunks.end();)
    {
        // bool x_test, z_test;
        bool x_test = true ? it->first.x < playerPos.x - ((RENDER_DISTANCE + 1) * CHUNK_SIZE) || it->first.x > playerPos.x + ((RENDER_DISTANCE + 1) * CHUNK_SIZE) : false;
        bool z_test = true ? it->first.z < playerPos.z - ((RENDER_DISTANCE + 1) * CHUNK_SIZE) || it->first.z > playerPos.z + ((RENDER_DISTANCE + 1) * CHUNK_SIZE) : false;

        if(x_test || z_test)
        {
            // Unload chunk
            // UnloadChunk(it->first);
            // it++;
            std::cout << "Unloading chunk\t" << it->first.x << ", " << it->first.z << "\n";
            m_Faces -= it->second->m_Faces;
            delete it->second;
            it = m_Chunks.erase(it);
        }
        else
            it++;
    }

    return true;
}