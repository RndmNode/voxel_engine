// #include "Voxel.h"
#define VOXEL_SIZE 1.0f

#define CHUNK_SIZE 32
#define CHUNK_AREA CHUNK_SIZE * CHUNK_SIZE
#define CHUNK_VOLUME CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

enum VoxelType
{
    AIR,
    STONE
};

struct Voxel 
{
    bool m_Active = true;
    glm::vec3 m_Position;
    glm::vec4 m_Color;
    VoxelType m_Type;
};

struct Mesh
{
    std::vector<glm::vec3> m_Vertices;
    std::vector<glm::vec3> m_Normals;
    std::vector<glm::vec2> m_TexCoords;
    std::vector<unsigned int> m_Indices;
};

class Chunk
{
public:
    Chunk();
    ~Chunk();
    void OnRender();
    void OnUpdate();
    void SetVoxelsActiveState();

    bool m_AllVoxelsActive = true;

    Voxel m_Voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
private:
    // static const int CHUNK_SIZE = 16;
    

};