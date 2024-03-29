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

// struct comp
// {
//     bool operator()(const ChunkPosition& loc1, const ChunkPosition& loc2) const
//     {
//         //Compare the 2 locations, return true if loc1 is less than loc2
//         return loc1.x < loc2.x || (loc1.x == loc2.x && loc1.z < loc2.z);
//     }
// };

using ChunkMap = std::unordered_map<ChunkPosition, Chunk*, MyHashFunction>;

class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();
    void CompileMeshes();
    unsigned int GetFaces() { return m_Faces; }

    Mesh* m_Mesh;
private:
    ChunkMap m_Chunks;
    unsigned int m_Faces = 0;
};

