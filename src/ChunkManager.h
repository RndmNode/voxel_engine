#include "Chunk.h"

#include <unordered_map>

class MyHashFunction {
public:
 
    // Custom hash function for storing chunks in an unordered_map
    size_t operator()(const ChunkPosition& pos) const
    {
        return std::hash<int>()(pos.x) ^ std::hash<int>()(pos.z);
    }
};

using ChunkMap = std::unordered_map<ChunkPosition, Chunk*, MyHashFunction>;

class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();
    void CompileMeshes();
    void BuildMesh();
    unsigned int GetFaces() { return m_Faces; }

    Mesh* m_Mesh;
private:
    ChunkMap m_Chunks;
    unsigned int m_Faces = 0;

    NeighborList GetNeighbors(glm::ivec3 voxelPos, ChunkPosition chunkPos);
};

