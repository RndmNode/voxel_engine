#pragma once

#include "Test.h"

#include "../Voxel.h"
#include "../vertex_array.h"
#include "../index_buffer.h"
#include "../vertex_buffer.h"
#include "../shader.h"
#include "../Texture.h"
#include "../Chunk.h"

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
        void SetMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;
        // void ProcessInput(GLFWwindow *window) override;
    private:
        // Geometry buffers
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;
        unsigned int m_InstanceBuffer;

        // Render settings
        bool m_wire_toggle = false;
        bool m_mouse_captured = true;
        // bool m_AllVoxelsActive = true;
        // glm::mat4 m_Model = glm::mat4(1.0);

        // Voxel m_Voxel;
        Chunk* m_Chunk;
    };
} // namespace test
