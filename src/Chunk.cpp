#include "Chunk.h"


Chunk::Chunk()
{
    // Create voxels
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                m_Voxels[i][j][k] = Voxel();
                if (m_AllVoxelsActive)
                {
                    m_Voxels[i][j][k].m_Active = true;
                } else {
                    if (i == 0 || j == 0 || k == 0 || i == CHUNK_SIZE - 1 || j == CHUNK_SIZE - 1 || k == CHUNK_SIZE - 1)
                    {
                        m_Voxels[i][j][k].m_Active = true;
                    }
                }
            }
        }
    }
}

Chunk::~Chunk()
{
    // // Delete voxels
    // for (int i = 0; i < CHUNK_SIZE; i++)
    // {
    //     for (int j = 0; j < CHUNK_SIZE; j++)
    //     {
    //         for (int k = 0; k < CHUNK_SIZE; k++)
    //         {
    //             delete m_Voxels[i][j][k];
    //         }
    //     }
    // }
}

void Chunk::OnRender()
{
    
}

void Chunk::OnUpdate()
{
    
}

void Chunk::SetVoxelsActiveState()
{
    for (int i = 0; i < CHUNK_SIZE; i++)
    {
        for (int j = 0; j < CHUNK_SIZE; j++)
        {
            for (int k = 0; k < CHUNK_SIZE; k++)
            {
                if (!m_AllVoxelsActive){
                    if (i == 0 || j == 0 || k == 0 || i == CHUNK_SIZE - 1 || j == CHUNK_SIZE - 1 || k == CHUNK_SIZE - 1)
                    {
                        m_Voxels[i][j][k].m_Active = true;
                    } else {
                        m_Voxels[i][j][k].m_Active = false;
                    }
                } else {
                    m_Voxels[i][j][k].m_Active = true;
                }
            }
        }
    }
}
