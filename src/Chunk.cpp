#include "Chunk.h"

#include <iostream>

#define NOISE_RESOLUTION 0.01f

Chunk::Chunk(ChunkPosition position, siv::PerlinNoise::seed_type seed)
    :   m_Position(position)
{
    // Generate height map
    BuildHeightMap(seed);

    // Generate voxels
    for (int i = 0; i < CHUNK_AREA; i++)
    {
        // Get voxel position
        int x = i % CHUNK_SIZE;
        int z = (i / CHUNK_SIZE) % CHUNK_SIZE;

        // Get height value
        float height = ceil(heightMap[x][z] * CHUNK_SIZE);

        // Set voxels in x & z column based on height value
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            if (y > height)
                m_Voxels[x][y][z] = new Voxel::VoxelData({Voxel::AIR});
            else
                m_Voxels[x][y][z] = new Voxel::VoxelData({Voxel::SOLID});
        }
    }
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

void Chunk::BuildHeightMap(siv::PerlinNoise::seed_type seed)
{
    // Instantiate PerlinNoise object
    const siv::PerlinNoise perlin{ seed };

    // Get height for each x, z coordinate
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            // Get height value
            float height = perlin.octave2D_01((x + m_Position.x) * NOISE_RESOLUTION, (z + m_Position.z) * NOISE_RESOLUTION, 4);

            // Store height value
            heightMap[x][z] = height;
        }
    }
}