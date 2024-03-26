#include "Voxel.h"

#define CHUNK_SIZE 4
#define CHUNK_AREA CHUNK_SIZE * CHUNK_SIZE
#define CHUNK_VOLUME CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

// #include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

#include "Voxel.h"

struct Mesh
{
    std::vector<glm::vec4> m_Instances;
    std::vector<float> m_Normals;
    std::vector<float> m_TexCoords;
    std::vector<unsigned int> m_Indices;
};

using NeighborList = std::vector< std::tuple<Voxel::VoxelFace, Voxel::VoxelType>>;

class Chunk
{
public:
    Chunk();
    ~Chunk();
    void OnRender();
    void OnUpdate();
    void BuildMesh();
    // unsigned int GetIndiciesCount() { return m_Indicies_count; }
    // Mesh GetMesh() { return m_Mesh; }
    Mesh m_Mesh;
    int GetFaces() { return m_Faces; }

    NeighborList GetNeighbors(uint8_t x, uint8_t y, uint8_t z);

    Voxel::VoxelData m_Voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    
private:
    // static const int CHUNK_SIZE = 16;
    
    int m_Faces = 0;
    // unsigned int m_Indicies_count = 0;

    // const std::unordered_map<VoxelFace, std::array<float, 12>> m_FACE_VERTICES;
};