#include "TestInstancing.h"

const float FRONT_FACE_VERTICES[] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f
};

namespace test {
    TestInstancing::TestInstancing()
    {
        glm::vec3 translations[m_InstanceCount];
        float offset_x = 0.0f;
        float offset_y = 0.0f;
        for (int i = 0; i < m_InstanceCount; i++)
        {
            glm::vec3 translation;
            translation.x = offset_x + 0.1f;
            translation.y = offset_y + 0.1f;
            translation.z = 0.0f;
            translations[i] = translation;

            offset_x += 1.0f;
            offset_y += 1.0f;
        }

        // Generate Instance Buffer
        GLCall(glGenBuffers(1, &m_InstanceBuffer));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(FRONT_FACE_VERTICES) * m_InstanceCount, &translations[0], GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

        // Generate Vertex Array and Vertex Buffer
        GLCall(glGenVertexArrays(1, &m_VertexArray));
        GLCall(glGenBuffers(1, &m_VertexBuffer));

        // Bind Vertex Array and Vertex Buffer
        GLCall(glBindVertexArray(m_VertexArray));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));

        // Fill Vertex Buffer with data
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(FRONT_FACE_VERTICES), FRONT_FACE_VERTICES, GL_STATIC_DRAW));

        // Set Vertex Attributes
        GLCall(glEnableVertexAttribArray(0));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        GLCall(glEnableVertexAttribArray(1));
        glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(1, 1);

        // Set Shader
        m_Shader = std::make_unique<Shader>("res/shaders/simple.shader");
        m_Shader->Bind();
    }
    
    TestInstancing::~TestInstancing()
    {
        GLCall(glDeleteVertexArrays(1, &m_VertexArray));
        GLCall(glDeleteBuffers(1, &m_VertexBuffer));
    }
    
    void TestInstancing::OnUpdate(GLFWwindow *window, float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            m_wire_toggle = !m_wire_toggle;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            m_mouse_captured = !m_mouse_captured;
    }
    
    void TestInstancing::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        if (m_mouse_captured)
            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        
        if (m_wire_toggle)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        m_Shader->Bind();
        GLCall(glBindVertexArray(m_VertexArray));
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_InstanceCount);
        GLCall(glBindVertexArray(0));
    }
    
    void TestInstancing::OnImGuiRender()
    {
        ImGui::Text("Press 'F' to toggle wireframe mode: %s", (m_wire_toggle ? "on" : "off"));
        ImGui::Text("Press 'C' to toggle mouse capture: %s", (m_mouse_captured ? "on" : "off"));
        ImGui::Text("\nApplication average %.3f ms/frame (%.1f FPS)", 
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // framerate
    }
    
    void TestInstancing::SetMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniformMat4f("u_View", view);
        m_Shader->SetUniformMat4f("u_Projection", projection);
    }
}