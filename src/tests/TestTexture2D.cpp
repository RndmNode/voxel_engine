// #include <GL/glew.h>

#include "TestTexture2D.h"
#include "../renderer.h"

namespace test {
    TestTexture2D::TestTexture2D()
        : m_Projection(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),       // projection matrix
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),    // view matrix
          m_TranslationA(200.0f, 200.0f, 0.0f), m_TranslationB(400.0f, 200.0f, 0.0f)
    {
        // Triangle
        // --------
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
             50.0f, -50.0f, 1.0f, 0.0f, // 1
             50.0f,  50.0f, 1.0f, 1.0f, // 2
            -50.0f,  50.0f, 0.0f, 1.0f, // 3
        };
        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Enable blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IBO = std::make_unique<IndexBuffer>(indicies, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/test.shader");
        m_Shader->Bind();
        std::string path = "res/textures/space.png";
        m_Texture = std::make_unique<Texture>(path);
        m_Shader->SetUniform1i("u_Texture", 0);
        
    }
    
    TestTexture2D::~TestTexture2D()
    {
    }
    
    void TestTexture2D::OnUpdate(GLFWwindow *window, float deltaTime)
    {
    }
    
    void TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        m_Texture->Bind();

         {
            // model view matrix as a slider
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);                 // Model Matrix
            glm::mat4 mvp = m_Projection * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp); // Set m_Projection matrix
            // draw
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }

        {
            // model view matrix as a slider
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);                 // Model Matrix
            glm::mat4 mvp = m_Projection * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp); // Set projection matrix
            // draw
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
    }
    
    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);            // Edit MVP matrix
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);            // Edit MVP matrix
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}