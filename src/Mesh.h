#pragma once

#include <vector>
#include "vendor/glm/glm.hpp"

struct Mesh
{
    std::vector<glm::vec4> m_Instances;
    std::vector<float> m_Normals;
    std::vector<float> m_TexCoords;
    std::vector<unsigned int> m_Indices;
};