// #include <GL/glew.h>

#include "TestBatchColorQuads.h"
#include "../renderer.h"
#include "../vendor/imgui/imgui.h"

namespace test {
    TestBatchColorQuads::TestBatchColorQuads()
        : m_Projection(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),       // projection matrix
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))     // view matrix
    {
        // Two Quads
        // --------
        float positions[] = {
          //  x      y      z          r     g     b      a
            -0.75f, -0.25f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,  // 0
            -0.25f, -0.25f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,  // 1
            -0.25f,  0.25f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,  // 2
            -0.75f,  0.25f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,  // 3

             0.25f, -0.25f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,  // 4
             0.75f, -0.25f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,  // 5
             0.75f,  0.25f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,  // 6
             0.25f,  0.25f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f   // 7
        };
        unsigned int indicies[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        // Build layout for vertex buffer
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(4);

        // Set vertex buffer
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 7 * 8 * sizeof(float));
        m_VAO = std::make_unique<VertexArray>();
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        // Set Index buffer
        m_IBO = std::make_unique<IndexBuffer>(indicies, 12);

        // Set Shader
        m_Shader = std::make_unique<Shader>("res/shaders/tests/BatchQuads.shader");
        m_Shader->Bind();
    }
    
    TestBatchColorQuads::~TestBatchColorQuads()
    {
    }
    
    void TestBatchColorQuads::OnUpdate(GLFWwindow *window, float deltaTime)
    {
    }
    
    void TestBatchColorQuads::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }
    
    void TestBatchColorQuads::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // framerate
    }
}