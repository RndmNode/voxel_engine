#pragma once

#include "Test.h"

#include "../vertex_buffer.h"
#include "../vertex_buffer_layout.h"
#include "../Texture.h"

#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/imgui/imgui.h"

#include <memory>

namespace test
{
    class TestBatchTextures : public Test {
    public:
        TestBatchTextures();
        ~TestBatchTextures();

        void OnUpdate(GLFWwindow *window, float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        std::vector<std::unique_ptr<Texture>> textures;
        glm::mat4 m_Projection, m_View; // projection and view matricies

        float m_Quad_1_Position[2] = { -0.75f, -0.25f };
    };
} // namespace test
