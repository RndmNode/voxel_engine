#pragma once

#define VOXEL_SIZE 1.0f

#include <string>

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

    inline std::string VoxelFaceToString(VoxelFace face)
    {
        switch (face)
        {
            case FRONT:     return "FRONT";
            case BACK:      return "BACK";
            case LEFT:      return "LEFT";
            case RIGHT:     return "RIGHT";
            case TOP:       return "TOP";
            case BOTTOM:    return "BOTTOM";
            default:        return "UNKNOWN";
        }
    };

    enum VoxelType
    {
        AIR,
        SOLID
    };

    struct VoxelData
    {
        VoxelType m_Type;
    };
}