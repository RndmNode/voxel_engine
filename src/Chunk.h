#pragma once

#include "Voxel.h"
#include "Mesh.h"
#include "vendor/PerlinNoise.hpp"

#define CHUNK_SIZE 32
#define CHUNK_AREA CHUNK_SIZE * CHUNK_SIZE
#define CHUNK_VOLUME CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

#include <vector>

struct ChunkPosition
{
    ChunkPosition(int x=0, int z=0) : x(x), z(z) {}
    int x, z;

    bool operator==(const ChunkPosition& other) const
    {
        return x == other.x && z == other.z;
    }

    bool operator!=(const ChunkPosition& other) const
    {
        return x != other.x || z != other.z;
    }
};

struct Neighbor
{
    Voxel::VoxelFace m_Face;
    Voxel::VoxelType m_Type;
};

using NeighborList = std::vector<Neighbor>;


class ChunkManager; // Forward declaration to use the ChunkManager class in the Chunk class

class Chunk
{
public:
    Chunk(ChunkPosition position, siv::PerlinNoise::seed_type seed, ChunkManager* manager);
    ~Chunk();
    void OnRender();
    void OnUpdate();
    void BuildMesh();

    Voxel::VoxelData* m_Voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    Mesh* m_Mesh;
    int m_Faces = 0;
private:
    void BuildHeightMap();
    NeighborList GetNeighbors(glm::ivec3 voxelPos);
    Neighbor GetOutOfBoundsNeighbor(glm::vec3 voxelPos, Voxel::VoxelFace face);
    siv::PerlinNoise::seed_type m_Seed;

    ChunkPosition m_Position;
    int heightMap[CHUNK_SIZE][CHUNK_SIZE];
    ChunkManager* m_Manager;
};