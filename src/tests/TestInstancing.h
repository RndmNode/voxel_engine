#pragma once

#include "Test.h"

#include "../renderer.h"
#include "../vendor/imgui/imgui.h"
#include "../vertex_buffer_layout.h"

namespace test
{
    class TestInstancing : public Test {
    public:
        TestInstancing();
        ~TestInstancing();

        void OnUpdate(GLFWwindow *window, float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;
    private:
        unsigned int m_InstanceCount = 25;

        unsigned int m_VertexBuffer;
        unsigned int m_VertexArray;
        unsigned int m_IndexBuffer;
        unsigned int m_InstanceBuffer;

        std::unique_ptr<Shader> m_Shader;

        // Render settings
        bool m_wire_toggle = false;
        bool m_mouse_captured = true;
    };
} // namespace test
