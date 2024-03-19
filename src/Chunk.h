#include "Voxel.h"

#define CHUNK_SIZE 16

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