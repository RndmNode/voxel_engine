#include "ChunkManager.h"
#include <iostream>

ChunkManager::ChunkManager(siv::PerlinNoise::seed_type seed)
    :   m_Mesh(new Mesh()),
        m_World_Seed(seed)
{
    // m_Chunks.insert({ChunkPosition(0, 0), new Chunk(ChunkPosition(0, 0))});
    // m_Chunks.insert({ChunkPosition(CHUNK_SIZE, 0), new Chunk(ChunkPosition(CHUNK_SIZE, 0))});
    // m_Chunks.insert({ChunkPosition(CHUNK_SIZE, -CHUNK_SIZE), new Chunk(ChunkPosition(CHUNK_SIZE, -CHUNK_SIZE))});
    // m_Chunks.insert({ChunkPosition(0, CHUNK_SIZE), new Chunk(ChunkPosition(0, CHUNK_SIZE))});
    // m_Chunks.insert({ChunkPosition(-CHUNK_SIZE, CHUNK_SIZE), new Chunk(ChunkPosition(-CHUNK_SIZE, CHUNK_SIZE))});

    // m_Chunks.insert({ChunkPosition(0, 0), new Chunk(ChunkPosition(0, 0), m_World_Seed)});
    // m_Chunks.insert({ChunkPosition(CHUNK_SIZE, 0), new Chunk(ChunkPosition(CHUNK_SIZE, 0), m_World_Seed)});
    // m_Chunks.insert({ChunkPosition(0, CHUNK_SIZE), new Chunk(ChunkPosition(0, CHUNK_SIZE), m_World_Seed)});
    // m_Chunks.insert({ChunkPosition(CHUNK_SIZE, CHUNK_SIZE), new Chunk(ChunkPosition(CHUNK_SIZE, CHUNK_SIZE), m_World_Seed)});

    for (int i = -5; i < 5; i++)
    {
        for (int j = -5; j < 5; j++)
        {
            m_Chunks.insert({ChunkPosition(i * CHUNK_SIZE, j * CHUNK_SIZE), new Chunk(ChunkPosition(i * CHUNK_SIZE, j * CHUNK_SIZE), m_World_Seed)});
        }
    }
}

ChunkManager::~ChunkManager()
{
    // Clean up
    for (auto& chunk : m_Chunks)
    {
        delete chunk.second;
    }
    delete m_Mesh;
}

void ChunkManager::BuildMesh()
{
    // Loop through all loaded chunks
    for (auto & chunk : m_Chunks)
    {
        // Loop through all voxels in chunk
        for (int i = 0; i < CHUNK_VOLUME; i++)
        {
            // Get voxel position
            int x = i % CHUNK_SIZE;
            int y = i / (CHUNK_SIZE * CHUNK_SIZE);
            int z = (i / CHUNK_SIZE) % CHUNK_SIZE;

            // Get voxel type
            if (chunk.second->m_Voxels[x][y][z]->m_Type == Voxel::AIR)
            {
                continue;
            }

            // Get neighbors
            NeighborList neighbors = GetNeighbors({x, y, z}, chunk.first);

            // Loop through all neighbors to check if face is visible
            for (auto neighbor : neighbors)
            {
                // Get neighbor face and type
                Voxel::VoxelFace face = neighbor.m_Face;
                Voxel::VoxelType type = neighbor.m_Type;

                if (type == Voxel::AIR)
                {
                    // Add face to mesh
                    m_Mesh->m_Instances.push_back(glm::vec4(x + chunk.first.x, y, z + chunk.first.z, float(face)));

                    // Increment face count
                    m_Faces++;
                }
            }

        }
    }
}

NeighborList ChunkManager::GetNeighbors(glm::ivec3 voxelPos, ChunkPosition chunkPos)
{
    NeighborList neighbors;

    // Add neighbors that are out of bounds
    if (voxelPos.x <= 0)
    {
        const auto& chunk =  m_Chunks.find({chunkPos.x - CHUNK_SIZE, chunkPos.z});

        if (chunk != m_Chunks.end())        neighbors.push_back({Voxel::VoxelFace::LEFT,    chunk->second->m_Voxels[CHUNK_SIZE - 1][voxelPos.y][voxelPos.z]->m_Type});
        else                                neighbors.push_back({Voxel::VoxelFace::LEFT,    Voxel::VoxelType::AIR});
    }

    if (voxelPos.x >= CHUNK_SIZE - 1)
    {
        const auto& chunk =  m_Chunks.find({chunkPos.x + CHUNK_SIZE, chunkPos.z});

        if (chunk != m_Chunks.end())        neighbors.push_back({Voxel::VoxelFace::RIGHT,   chunk->second->m_Voxels[0][voxelPos.y][voxelPos.z]->m_Type});
        else                                neighbors.push_back({Voxel::VoxelFace::RIGHT,   Voxel::VoxelType::AIR});
    }

    if (voxelPos.y <= 0)                    neighbors.push_back({Voxel::VoxelFace::BOTTOM,  Voxel::VoxelType::AIR});
    if (voxelPos.y >= CHUNK_SIZE - 1)       neighbors.push_back({Voxel::VoxelFace::TOP,     Voxel::VoxelType::AIR});

    if (voxelPos.z <= 0)
    {
        const auto& chunk =  m_Chunks.find({chunkPos.x, chunkPos.z - CHUNK_SIZE});

        if (chunk != m_Chunks.end())        neighbors.push_back({Voxel::VoxelFace::BACK,    chunk->second->m_Voxels[voxelPos.x][voxelPos.y][CHUNK_SIZE - 1]->m_Type});
        else                                neighbors.push_back({Voxel::VoxelFace::BACK,    Voxel::VoxelType::AIR});
    }

    if (voxelPos.z >= CHUNK_SIZE - 1)
    {
        const auto& chunk =  m_Chunks.find({chunkPos.x, chunkPos.z + CHUNK_SIZE});

        if (chunk != m_Chunks.end())        neighbors.push_back({Voxel::VoxelFace::FRONT,   chunk->second->m_Voxels[voxelPos.x][voxelPos.y][0]->m_Type});
        else                                neighbors.push_back({Voxel::VoxelFace::FRONT,   Voxel::VoxelType::AIR});
    }

    // Add neighbors that are in bounds
    if (voxelPos.x > 0)                   neighbors.push_back({Voxel::VoxelFace::LEFT,    m_Chunks[chunkPos]->m_Voxels[voxelPos.x- 1][voxelPos.y][voxelPos.z]->m_Type});
    if (voxelPos.x < CHUNK_SIZE - 1)      neighbors.push_back({Voxel::VoxelFace::RIGHT,   m_Chunks[chunkPos]->m_Voxels[voxelPos.x + 1][voxelPos.y][voxelPos.z]->m_Type});
    if (voxelPos.y > 0)                   neighbors.push_back({Voxel::VoxelFace::BOTTOM,  m_Chunks[chunkPos]->m_Voxels[voxelPos.x][voxelPos.y - 1][voxelPos.z]->m_Type});
    if (voxelPos.y < CHUNK_SIZE - 1)      neighbors.push_back({Voxel::VoxelFace::TOP,     m_Chunks[chunkPos]->m_Voxels[voxelPos.x][voxelPos.y + 1][voxelPos.z]->m_Type});
    if (voxelPos.z > 0)                   neighbors.push_back({Voxel::VoxelFace::BACK,    m_Chunks[chunkPos]->m_Voxels[voxelPos.x][voxelPos.y][voxelPos.z - 1]->m_Type});
    if (voxelPos.z < CHUNK_SIZE - 1)      neighbors.push_back({Voxel::VoxelFace::FRONT,   m_Chunks[chunkPos]->m_Voxels[voxelPos.x][voxelPos.y][voxelPos.z + 1]->m_Type});

    return neighbors;
}