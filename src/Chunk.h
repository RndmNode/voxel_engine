#include "Voxel.h"

#define CHUNK_SIZE 1
#define CHUNK_AREA CHUNK_SIZE * CHUNK_SIZE
#define CHUNK_VOLUME CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

// #include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

struct Mesh
{
    std::vector<float> m_Vertices;
    std::vector<float> m_Normals;
    std::vector<float> m_TexCoords;
    std::vector<unsigned int> m_Indices;
};

using NeighborList = std::vector< std::tuple<VoxelFace, VoxelType>>;

class Chunk
{
public:
    Chunk();
    ~Chunk();
    void OnRender();
    void OnUpdate();
    void BuildMesh();
    unsigned int GetIndiciesCount() { return m_Indicies_count; }
    Mesh GetMesh() { return m_Mesh; }
    int GetFaces() { return m_Faces; }

    NeighborList GetNeighbors(uint8_t x, uint8_t y, uint8_t z);
    // void SetVoxelsActiveState();

    bool m_AllVoxelsActive = true;

    Voxel m_Voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    // std::vector<float[12]> m_FACE_VERTICES;
    
    
private:
    // static const int CHUNK_SIZE = 16;
    Mesh m_Mesh;
    int m_Faces = 0;
    unsigned int m_Indicies_count = 0;

    const std::unordered_map<VoxelFace, std::array<float, 12>> m_FACE_VERTICES;
};