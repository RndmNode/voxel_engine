#include "Chunk.h"
#include "Mesh.h"

#include <unordered_map>

class MyHashFunction {
public:
 
    // Custom hash function for storing chunks in an unordered_map
    size_t operator()(const ChunkPosition& pos) const
    {
        return std::hash<int>()(pos.x) ^ std::hash<int>()(pos.z);
    }
};

struct Neighbor
{
    Voxel::VoxelFace m_Face;
    Voxel::VoxelType m_Type;
};

using ChunkMap = std::unordered_map<ChunkPosition, Chunk*, MyHashFunction>;
using NeighborList = std::vector<Neighbor>;

class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();
    void BuildMesh();
    unsigned int GetFaces() { return m_Faces; }

    Mesh* m_Mesh;
private:
    ChunkMap m_Chunks;
    unsigned int m_Faces = 0;

    NeighborList GetNeighbors(glm::ivec3 voxelPos, ChunkPosition chunkPos);
};

