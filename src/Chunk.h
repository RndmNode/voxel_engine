#pragma once

#include "Voxel.h"
#include "vendor/PerlinNoise.hpp"

#define CHUNK_SIZE 32
#define CHUNK_AREA CHUNK_SIZE * CHUNK_SIZE
#define CHUNK_VOLUME CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

#include <vector>
#include <unordered_map>

struct ChunkPosition
{
    ChunkPosition(int x, int z) : x(x), z(z) {}
    int x, z;

    bool operator==(const ChunkPosition& other) const
    {
        return x == other.x && z == other.z;
    }
};

class Chunk
{
public:
    Chunk(ChunkPosition position, siv::PerlinNoise::seed_type seed);
    ~Chunk();
    void OnRender();
    void OnUpdate();
    void BuildHeightMap(siv::PerlinNoise::seed_type seed);

    Voxel::VoxelData* m_Voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
private:
    ChunkPosition m_Position;
    float heightMap[CHUNK_SIZE][CHUNK_SIZE];
};