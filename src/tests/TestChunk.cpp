#include "TestChunk.h"

#include "../vertex_buffer_layout.h"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../Camera.h"

namespace test {
    TestChunk::TestChunk()
        : m_Chunk(new Chunk())
    {
        std::cout << m_Chunk->GetFaces() << std::endl;
        
        // Generate Instance Buffer
        GLCall(glGenBuffers(1, &m_InstanceBuffer));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel::FRONT_FACE_VERTICES) * m_Chunk->GetFaces(), &m_Chunk->m_Mesh->m_Instances[0], GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

        // Generate Vertex Array and Vertex Buffer
        GLCall(glGenVertexArrays(1, &m_VertexArray));
        GLCall(glGenBuffers(1, &m_VertexBuffer));
        GLCall(glGenBuffers(1, &m_IndexBuffer));

        // Bind Vertex Array and Vertex Buffer
        GLCall(glBindVertexArray(m_VertexArray));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));

        // Fill Vertex Buffer with data
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel::FRONT_FACE_VERTICES), Voxel::FRONT_FACE_VERTICES, GL_STATIC_DRAW));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Voxel::indices), Voxel::indices, GL_STATIC_DRAW));

        // Set Vertex Attributes
        GLCall(glEnableVertexAttribArray(0));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        GLCall(glEnableVertexAttribArray(1));
        glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(0, 0);
        glVertexAttribDivisor(1, 1);

        // Set Shader
        m_Shader = std::make_unique<Shader>("res/shaders/simple.shader");
        m_Shader->Bind();

        // Enable Depth Buffer
        GLCall(glEnable(GL_DEPTH_TEST));

        // Enable Face Culling
        GLCall(glEnable(GL_CULL_FACE));
    }
    
    TestChunk::~TestChunk()
    {
    }
    
    void TestChunk::OnUpdate(GLFWwindow *window, float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            m_wire_toggle = !m_wire_toggle;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            m_mouse_captured = !m_mouse_captured;
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
            cull_face = !cull_face;
    }
    
    void TestChunk::OnRender()
    {
        // Clear the screen
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // If mouse toggle, capture mouse
        if (m_mouse_captured)
            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        
        // If wireframe toggle, set to wireframe
        if (m_wire_toggle) {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        } else {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        }

        // Face Culling
        if (cull_face)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);

        m_Shader->Bind();
        GLCall(glBindVertexArray(m_VertexArray));
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, m_Chunk->GetFaces());
        GLCall(glBindVertexArray(0));
    }
    
    void TestChunk::OnImGuiRender()
    {
        ImGui::Text("Press 'F' to toggle wireframe mode: %s", (m_wire_toggle ? "on" : "off"));
        ImGui::Text("Press 'C' to toggle mouse capture: %s", (m_mouse_captured ? "on" : "off"));
        ImGui::Text("Press 'V' to toggle face culling: %s", (cull_face ? "on" : "off"));
        ImGui::Text("Faces: %d", m_Chunk->GetFaces());
        ImGui::Text("\nApplication average %.3f ms/frame (%.1f FPS)", 
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // framerate
    }
    
    void TestChunk::SetMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniformMat4f("u_View", view);
        m_Shader->SetUniformMat4f("u_Projection", projection);
    }
}