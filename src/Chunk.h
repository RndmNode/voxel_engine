#include "Voxel.h"

#define CHUNK_SIZE 32
#define CHUNK_AREA CHUNK_SIZE * CHUNK_SIZE
#define CHUNK_VOLUME CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

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

struct Neighbor
{
    Voxel::VoxelFace m_Face;
    Voxel::VoxelType m_Type;
};

using NeighborList = std::vector<Neighbor>;

class Chunk
{
public:
    Chunk();
    ~Chunk();
    void OnRender();
    void OnUpdate();
    void BuildMesh();
    int GetFaces() { return m_Faces; }

    NeighborList GetNeighbors(int x, int y, int z);

    Voxel::VoxelData m_Voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    Mesh m_Mesh;
private:
    int m_Faces = 0;
};