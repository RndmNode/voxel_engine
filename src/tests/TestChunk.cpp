#include "TestChunk.h"

#include "../vertex_buffer_layout.h"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../Camera.h"

const std::array<float, 12> FRONT_FACE_VERTICES = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

namespace test {
    TestChunk::TestChunk()
        : m_Chunk(new Chunk())
    {
        

        // Enable Depth Buffer
        GLCall(glEnable(GL_DEPTH_TEST));

        // Enable Face Culling
        // GLCall(glEnable(GL_CULL_FACE));
        // GLCall(glCullFace(GL_FRONT));
        // GLCall(glFrontFace(GL_CW));
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
        // if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        //     m_Chunk->m_AllVoxelsActive = !m_Chunk->m_AllVoxelsActive;
        //     m_Chunk->SetVoxelsActiveState();
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
    }
    
    void TestChunk::OnImGuiRender()
    {
        ImGui::Text("Press 'F' to toggle wireframe mode: %s", (m_wire_toggle ? "on" : "off"));
        ImGui::Text("Press 'C' to toggle mouse capture: %s", (m_mouse_captured ? "on" : "off"));
        // ImGui::Text("Press 'V' to toggle all voxels active: %s", (m_Chunk->m_AllVoxelsActive ? "on" : "off"));
        ImGui::Text("Faces: %d", m_Chunk->GetFaces());
        ImGui::Text("Indicies: %d", m_Chunk->GetIndiciesCount());
        ImGui::Text("Vertices: %d", m_Chunk->GetMesh().m_Vertices.size() / 3);
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