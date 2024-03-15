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
    class TestTexture2D : public Test {
    public:
        TestTexture2D();
        ~TestTexture2D();

        void OnUpdate(GLFWwindow *window, float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        glm::mat4 m_Projection, m_View;
        glm::vec3 m_TranslationA, m_TranslationB;
    };
} // namespace test
