#include "Chunk.h"

#include <unordered_map>

class MyHashFunction {
public:
 
    // Custom hash function for storing chunks in an unordered_map
    size_t operator()(const glm::ivec3& pos) const
    {
        return std::hash<int>()(pos.x) ^ std::hash<int>()(pos.y) ^ std::hash<int>()(pos.z);
    }
};

using ChunkMap = std::unordered_map<glm::ivec3, Chunk*, MyHashFunction>;

class ChunkManager
{
public:
    ChunkManager();
    ~ChunkManager();

private:
    ChunkMap m_Chunks;
};

