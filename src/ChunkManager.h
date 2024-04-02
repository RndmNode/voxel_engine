#pragma once

#include "Chunk.h"

#include <unordered_map>

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
    void CompileMesh();
    // void BuildMesh();
    unsigned int GetFaces() { return m_Faces; }

    Mesh* m_Mesh;
    ChunkMap m_Chunks;
private:
    
    unsigned int m_Faces = 0;

    NeighborList GetNeighbors(glm::ivec3 voxelPos, ChunkPosition chunkPos);
    siv::PerlinNoise::seed_type m_World_Seed;
};

