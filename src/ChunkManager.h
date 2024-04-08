#pragma once

#include "Chunk.h"

#include <unordered_map>

#define RENDER_DISTANCE 4

class MyHashFunction {
public:
 
    // Custom hash function for storing chunks in an unordered_map
    size_t operator()(const ChunkPosition& pos) const
    {
        return (std::hash<int>()(pos.x) ^ std::hash<int>()(pos.z)) & 
               (std::hash<int>()(pos.x) & std::hash<int>()(pos.z));
    }
};

using ChunkMap = std::unordered_map<ChunkPosition, Chunk*, MyHashFunction>;

class ChunkManager
{
public:
    ChunkManager(siv::PerlinNoise::seed_type seed);
    ~ChunkManager();
    bool Update();

    unsigned int GetFaces() { return m_Faces; }
    unsigned int GetNumChunks() { return m_NumChunks; }

    unsigned int m_NumChunks = 0;

    Mesh* m_Mesh;
    ChunkMap m_Chunks;
    glm::vec3 m_PlayerPosition = glm::vec3(0.0f, 0.0f, 0.0f);
private:
    bool UpdateChunksAroundPlayer();
    // void UpdateChunkMeshes(std::vector<ChunkPosition> stack);
    void LoadChunk(ChunkPosition chunkPos);
    // void UnloadChunk(ChunkPosition chunkPos);

    unsigned int m_Faces = 0;
    

    // NeighborList GetNeighbors(glm::ivec3 voxelPos, ChunkPosition chunkPos);
    siv::PerlinNoise::seed_type m_World_Seed;
    ChunkPosition m_CurrentPlayerChunkPos = ChunkPosition(5, 5);
    ChunkPosition m_PreviousPlayerChunkPos = ChunkPosition(5, 5);
};

