#pragma once

#include "Test.h"

#include "../vertex_array.h"
#include "../index_buffer.h"
#include "../vertex_buffer.h"
#include "../shader.h"
#include "../Texture.h"
#include "../ChunkManager.h"

#include "../vendor/imgui/imgui.h"

#include <memory>

namespace test
{
    class TestChunkManager : public Test {
    public:
        TestChunkManager();
        ~TestChunkManager();

        void OnUpdate(GLFWwindow *window, float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;
    private:
        // Geometry buffers
        std::unique_ptr<Shader> m_Shader;

        unsigned int m_VertexBuffer;
        unsigned int m_VertexArray;
        unsigned int m_IndexBuffer;
        unsigned int m_InstanceBuffer;

        // Render settings
        bool m_wire_toggle = false;
        bool m_mouse_captured = true;
        bool cull_face = true;

        // Voxel m_Voxel;
        // Chunk* m_Chunk;
        ChunkManager* m_ChunkManager;
    };
} // namespace test
