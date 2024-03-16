#pragma once

#include "Test.h"

#include "../Voxel.h"
#include "../vertex_array.h"
#include "../index_buffer.h"
#include "../vertex_buffer.h"
#include "../shader.h"
#include "../Texture.h"

#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/imgui/imgui.h"

#include <memory>

namespace test
{
    class TestChunk : public Test {
    public:
        TestChunk();
        ~TestChunk();

        void OnUpdate(GLFWwindow *window, float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(GLFWwindow *window) override;
    private:
        // Geometry buffers
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        // Render settings
        bool m_wire_toggle = false;
        
        // Camera settings
        float m_FOV = 90.0f; // Field of View
        glm::vec3 m_Cam_Pos = { 0.0f, 0.0f, 3.0f };
        const glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        const glm::vec3 m_CameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
        glm::mat4 m_Projection, m_View;
        glm::mat4 m_Model = glm::mat4(1.0);

        Voxel m_Voxel;
    };
} // namespace test
