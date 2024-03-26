#include "TestInstancing.h"
#include "../Voxel.h"

// const float FRONT_FACE_VERTICES[] = {
// //   x     y     z   
//     0.0f, 0.0f, 0.0f, 
//     1.0f, 0.0f, 0.0f,
//     1.0f, 1.0f, 0.0f,
//     0.0f, 1.0f, 0.0f
// };
// const unsigned int indices[] = {
//     0, 1, 2,
//     2, 3, 0
// };

namespace test {
    TestInstancing::TestInstancing()
    {
        glm::vec4 translations[m_InstanceCount * m_NumFaces];
        float offset_x = 0.0f;
        float offset_y = 0.0f;
        int index = 0;
        for (int i = 0; i < m_InstanceCount; i++)
        {
            glm::vec4 translation;
            translation.x = offset_x;
            translation.y = offset_y;
            translation.z = 0.0f;
            translation.w = 0.0f;
            translations[index++] = translation;

            translation.w = float(Voxel::VoxelFace::BACK);
            translations[index++] = translation;

            translation.w = float(Voxel::VoxelFace::LEFT);
            translations[index++] = translation;

            translation.w = float(Voxel::VoxelFace::RIGHT);
            translations[index++] = translation;

            translation.w = float(Voxel::VoxelFace::TOP);
            translations[index++] = translation;

            translation.w = float(Voxel::VoxelFace::BOTTOM);
            translations[index++] = translation;

            offset_x++;
            offset_y++;
        }

        // Generate Instance Buffer
        GLCall(glGenBuffers(1, &m_InstanceBuffer));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Voxel::FRONT_FACE_VERTICES) * m_InstanceCount * m_NumFaces, &translations[0], GL_STATIC_DRAW));
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

        // Enable Render Settings
        GLCall(glEnable(GL_DEPTH_TEST));
        // GLCall(glEnable(GL_CULL_FACE));
        // GLCall(glCullFace(GL_FRONT));
        // GLCall(glFrontFace(GL_CW));
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
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
            cull_face = !cull_face;
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

        if (cull_face)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);

        m_Shader->Bind();
        GLCall(glBindVertexArray(m_VertexArray));
        // glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_InstanceCount);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, (m_InstanceCount * m_NumFaces));
        GLCall(glBindVertexArray(0));
    }
    
    void TestInstancing::OnImGuiRender()
    {
        ImGui::Text("Press 'F' to toggle wireframe mode: %s", (m_wire_toggle ? "on" : "off"));
        ImGui::Text("Press 'C' to toggle mouse capture: %s", (m_mouse_captured ? "on" : "off"));
        ImGui::Text("Press 'V' to toggle face culling: %s", (cull_face ? "on" : "off"));
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