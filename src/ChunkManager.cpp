#include "ChunkManager.h"
#include <iostream>

ChunkManager::ChunkManager()
    :  m_Mesh(new Mesh())
{
    // m_Chunks[ChunkPosition(0, 0)] = new Chunk(ChunkPosition(0, 0));
    // m_Chunks[ChunkPosition(CHUNK_SIZE, CHUNK_SIZE)] = new Chunk(ChunkPosition(CHUNK_SIZE, CHUNK_SIZE));
    m_Chunks.insert({ChunkPosition(0, 0), new Chunk(ChunkPosition(0, 0))});
    m_Chunks.insert({ChunkPosition(CHUNK_SIZE, 0), new Chunk(ChunkPosition(CHUNK_SIZE, 0))});
    m_Chunks.insert({ChunkPosition(CHUNK_SIZE, -CHUNK_SIZE), new Chunk(ChunkPosition(CHUNK_SIZE, -CHUNK_SIZE))});
    m_Chunks.insert({ChunkPosition(0, CHUNK_SIZE), new Chunk(ChunkPosition(0, CHUNK_SIZE))});
    m_Chunks.insert({ChunkPosition(-CHUNK_SIZE, CHUNK_SIZE), new Chunk(ChunkPosition(-CHUNK_SIZE, CHUNK_SIZE))});

    for (auto& chunk : m_Chunks)
    {
        printf("Chunk: %d, %d\n", chunk.first.x, chunk.first.z);
    }
}

ChunkManager::~ChunkManager()
{
    
}

void ChunkManager::CompileMeshes()
{

    for (auto& chunk : m_Chunks)
    {
        // printf("Chunk: %d, %d\n", chunk.first.x, chunk.first.z);

        // Get instances from chunks
        std::vector<glm::vec4> instances = chunk.second->m_Mesh->m_Instances;
        m_Mesh->m_Instances.insert(std::end(m_Mesh->m_Instances), std::begin(instances), std::end(instances));

        // Get Faces from chunks
        m_Faces += chunk.second->GetFaces();
    }
}