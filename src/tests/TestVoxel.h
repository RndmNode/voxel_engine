#pragma once

#include "Test.h"
#include "../Voxel.h"
#include "../vertex_array.h"
#include "../index_buffer.h"
#include "../vertex_buffer.h"
#include "../shader.h"
#include "../Texture.h"

#include "../vendor/glm/gtc/matrix_transform.hpp"

#include <memory>

namespace test
{
    class TestVoxel : public Test {
    public:
        TestVoxel();
        ~TestVoxel();

        void OnUpdate(GLFWwindow *window, float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(GLFWwindow *window) override;

        float m_VoxelPos[3];
    private:
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        Voxel m_Voxel;
        bool m_rotate_toggle = false;
        bool m_wire_toggle = false;

        float m_Scale = 1.0f;
        float m_Translation[3] = { 0.0f, 0.0f, 0.0f };
        float m_FOV = 90.0f; // Field of View
        glm::vec3 m_Cam_Pos = { 0.0f, 0.0f, 3.0f };

        const glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        const glm::vec3 m_CameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
        
        // float m_ScreenWidth, m_ScreenHeight;
        glm::mat4 m_Projection, m_View; // projection and view matricies
    };
} // namespace test
