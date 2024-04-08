#include "Chunk.h"
#include "ChunkManager.h"
#include <iostream>

#define NOISE_RESOLUTION 0.01f

Chunk::Chunk(ChunkPosition position, siv::PerlinNoise::seed_type seed, ChunkManager* manager)
    :   m_Mesh(new Mesh()),
        m_Position(position),
        m_Manager(manager)
{
    // Set Noise
    m_Perlin = siv::PerlinNoise(seed);

    // Generate height map
    BuildHeightMap();

    // Generate voxels
    for (int i = 0; i < CHUNK_AREA; i++)
    {
        // Get voxel position
        int x = i % CHUNK_SIZE;
        int z = (i / CHUNK_SIZE) % CHUNK_SIZE;

        // Get height value
        int height = heightMap[x][z];;

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
    // Clean up
    for (int x = 0; x < CHUNK_SIZE; x++)
        for (int y = 0; y < CHUNK_SIZE; y++)
            for (int z = 0; z < CHUNK_SIZE; z++)
                delete m_Voxels[x][y][z];

    delete m_Mesh;
}

void Chunk::OnRender()
{
}

void Chunk::OnUpdate()
{
}

void Chunk::BuildHeightMap()
{
    // Get height for each x, z coordinate
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            // store height value
            heightMap[x][z] = ceil(m_Perlin.normalizedOctave2D_01(
                                    (x + m_Position.x) * NOISE_RESOLUTION, 
                                    (z + m_Position.z) * NOISE_RESOLUTION, 4) * CHUNK_SIZE);
        }
    }
}

void Chunk::BuildMesh()
{
    // Loop through all voxels in chunk
    for (int i = 0; i < CHUNK_AREA; i++)
    {
        // Get column
        int x = i % CHUNK_SIZE;
        int z = (i / CHUNK_SIZE) % CHUNK_SIZE;

        // Loop through all voxels in column
        for (int y = heightMap[x][z]; y >=0; y--)
        {
            // Get neighbors
            NeighborList neighbors = GetNeighbors({x, y, z});

            // Loop through all neighbors to check if face is visible
            for (auto neighbor : neighbors)
            {
                // Get neighbor face and type
                Voxel::VoxelFace face = neighbor.m_Face;
                Voxel::VoxelType type = neighbor.m_Type;

                if (type == Voxel::AIR)
                {
                    // Add face to mesh
                    m_Mesh->m_Instances.push_back(glm::vec4(x + m_Position.x, y, z + m_Position.z, float(face)));

                    // Increment face count
                    m_Faces++;
                }
            }
        }
    }
}

NeighborList Chunk::GetNeighbors(glm::ivec3 voxelPos)
{
    NeighborList neighbors;

    // Add neighbors that are out of bounds
    if (voxelPos.x <= 0)
    {
        neighbors.push_back(GetOutOfBoundsNeighbor({voxelPos.x, voxelPos.y, voxelPos.z}, Voxel::VoxelFace::LEFT));
        // const auto& chunk =  m_Manager->m_Chunks.find({m_Position.x - CHUNK_SIZE, m_Position.z});

        // if (chunk != m_Manager->m_Chunks.end())         neighbors.push_back({Voxel::VoxelFace::LEFT,    chunk->second->m_Voxels[CHUNK_SIZE - 1][voxelPos.y][voxelPos.z]->m_Type});
        // // else                                            neighbors.push_back({Voxel::VoxelFace::LEFT,    Voxel::VoxelType::AIR});
        // else                                           neighbors.push_back(GetOutOfBoundsNeighbor({voxelPos.x, voxelPos.y, voxelPos.z}, Voxel::VoxelFace::LEFT));
    }

    if (voxelPos.x >= CHUNK_SIZE - 1)
    {
        neighbors.push_back(GetOutOfBoundsNeighbor({voxelPos.x, voxelPos.y, voxelPos.z}, Voxel::VoxelFace::RIGHT));
        // const auto& chunk =  m_Manager->m_Chunks.find({m_Position.x + CHUNK_SIZE, m_Position.z});

        // if (chunk != m_Manager->m_Chunks.end())         neighbors.push_back({Voxel::VoxelFace::RIGHT,   chunk->second->m_Voxels[0][voxelPos.y][voxelPos.z]->m_Type});
        // // else                                            neighbors.push_back({Voxel::VoxelFace::RIGHT,   Voxel::VoxelType::AIR});
        // else                                           neighbors.push_back(GetOutOfBoundsNeighbor({voxelPos.x, voxelPos.y, voxelPos.z}, Voxel::VoxelFace::RIGHT));
    }

    // if (voxelPos.y <= 0)                    neighbors.push_back({Voxel::VoxelFace::BOTTOM,  Voxel::VoxelType::AIR});
    // if (voxelPos.y >= CHUNK_SIZE - 1)       neighbors.push_back({Voxel::VoxelFace::TOP,     Voxel::VoxelType::AIR});

    if (voxelPos.z <= 0)
    {
        neighbors.push_back(GetOutOfBoundsNeighbor({voxelPos.x, voxelPos.y, voxelPos.z}, Voxel::VoxelFace::BACK));
        // const auto& chunk =  m_Manager->m_Chunks.find({m_Position.x, m_Position.z - CHUNK_SIZE});

        // if (chunk != m_Manager->m_Chunks.end())         neighbors.push_back({Voxel::VoxelFace::BACK,    chunk->second->m_Voxels[voxelPos.x][voxelPos.y][CHUNK_SIZE - 1]->m_Type});
        // // else                                            neighbors.push_back({Voxel::VoxelFace::BACK,    Voxel::VoxelType::AIR});
        // else                                           neighbors.push_back(GetOutOfBoundsNeighbor({voxelPos.x, voxelPos.y, voxelPos.z}, Voxel::VoxelFace::BACK));
    }

    if (voxelPos.z >= CHUNK_SIZE - 1)
    {
        neighbors.push_back(GetOutOfBoundsNeighbor({voxelPos.x, voxelPos.y, voxelPos.z}, Voxel::VoxelFace::FRONT));
        // const auto& chunk =  m_Manager->m_Chunks.find({m_Position.x, m_Position.z + CHUNK_SIZE});

        // if (chunk != m_Manager->m_Chunks.end())         neighbors.push_back({Voxel::VoxelFace::FRONT,   chunk->second->m_Voxels[voxelPos.x][voxelPos.y][0]->m_Type});
        // // else                                            neighbors.push_back({Voxel::VoxelFace::FRONT,   Voxel::VoxelType::AIR});
        // else                                           neighbors.push_back(GetOutOfBoundsNeighbor({voxelPos.x, voxelPos.y, voxelPos.z}, Voxel::VoxelFace::FRONT));
    }

    // Add neighbors that are in bounds
    if (voxelPos.x > 0)                   neighbors.push_back({Voxel::VoxelFace::LEFT,    m_Voxels[voxelPos.x- 1][voxelPos.y][voxelPos.z]->m_Type});
    if (voxelPos.x < CHUNK_SIZE - 1)      neighbors.push_back({Voxel::VoxelFace::RIGHT,   m_Voxels[voxelPos.x + 1][voxelPos.y][voxelPos.z]->m_Type});
    if (voxelPos.y > 0)                   neighbors.push_back({Voxel::VoxelFace::BOTTOM,  m_Voxels[voxelPos.x][voxelPos.y - 1][voxelPos.z]->m_Type});
    if (voxelPos.y < CHUNK_SIZE - 1)      neighbors.push_back({Voxel::VoxelFace::TOP,     m_Voxels[voxelPos.x][voxelPos.y + 1][voxelPos.z]->m_Type});
    if (voxelPos.z > 0)                   neighbors.push_back({Voxel::VoxelFace::BACK,    m_Voxels[voxelPos.x][voxelPos.y][voxelPos.z - 1]->m_Type});
    if (voxelPos.z < CHUNK_SIZE - 1)      neighbors.push_back({Voxel::VoxelFace::FRONT,   m_Voxels[voxelPos.x][voxelPos.y][voxelPos.z + 1]->m_Type});

    return neighbors;
}

Neighbor Chunk::GetOutOfBoundsNeighbor(glm::vec3 voxelPos, Voxel::VoxelFace face)
{
    // Convert voxel position to world position
    int x = voxelPos.x + m_Position.x;
    int z = voxelPos.z + m_Position.z;

    // Get Coords of needed voxel position data
    if (face == Voxel::VoxelFace::FRONT)
        z++;
    else if (face == Voxel::VoxelFace::BACK)
        z--;
    else if (face == Voxel::VoxelFace::LEFT)
        x--;
    else if (face == Voxel::VoxelFace::RIGHT)
        x++;

    // Get height value at lookup position
    int y = ceil(m_Perlin.normalizedOctave2D_01(x * NOISE_RESOLUTION, z * NOISE_RESOLUTION, 4) * CHUNK_SIZE);

    // Return neighbor data
    if (y + 1 > voxelPos.y)
        return {face, Voxel::VoxelType::SOLID};
    else
        return {face, Voxel::VoxelType::AIR};
}