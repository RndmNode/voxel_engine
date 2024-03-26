#pragma once

#define VOXEL_SIZE 1.0f

#include <array>

#include "vendor/glm/glm.hpp"

namespace Voxel {
    inline const float FRONT_FACE_VERTICES[] = {
    //   x     y     z   
        0.0f, 0.0f, 0.0f, 
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    inline const unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    enum VoxelFace
    {
        FRONT,
        BACK,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };

    enum VoxelType
    {
        AIR,
        SOLID
    };

    struct VoxelData
    {
        // bool m_Active = true;
        // glm::vec3 m_Position;
        // glm::vec4 m_Color;
        VoxelType m_Type;
    };
}