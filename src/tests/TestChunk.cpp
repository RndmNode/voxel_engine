#include "TestChunk.h"

#include "../vertex_buffer_layout.h"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../Camera.h"


unsigned int const voxel_indices[36] = {
    0, 1, 2, 1, 3, 4,
    5, 6, 3, 7, 3, 6,
    2, 4, 7, 0, 7, 6,
    0, 5, 1, 1, 5, 3,
    5, 0, 6, 7, 4, 3,
    2, 1, 4, 0, 2, 7
};

namespace test {
    TestChunk::TestChunk()
        : m_Voxel({glm::vec3(0.0f, 0.0f, 0.0f), 1.0f})
    {
        
        // Build layout for vertex buffer
        VertexBufferLayout layout;
        layout.Push<float>(3);          // vertices
        layout.Push<float>(2);          // texture coordinates

        // Set vertex buffer
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(float) * 8 * 5);
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        // Set Index buffer
        m_IndexBuffer = std::make_unique<IndexBuffer>(voxel_indices, 36);

        // Set Shader
        m_Shader = std::make_unique<Shader>("res/shaders/simple.shader");
        m_Shader->Bind();

        // Set Texture
        std::string path = "res/textures/Tile.png";
        m_Texture = std::make_unique<Texture>(path);
        m_Shader->SetUniform1i("u_Texture", 0);

        // Enable Depth Buffer
        GLCall(glEnable(GL_DEPTH_TEST));

        // Enable Face Culling
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        GLCall(glFrontFace(GL_CW));
        
        // Chunk
        m_Chunk = new Chunk();
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
            m_Chunk->m_AllVoxelsActive = !m_Chunk->m_AllVoxelsActive;
            m_Chunk->SetVoxelsActiveState();
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

        // Instantiate renderer
        Renderer renderer;
        m_Texture->Bind(0);

        // Set Dynamic Vertex Buffer
        m_VertexBuffer->Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 5 * 8, m_Voxel.m_Vertices));

        // Loop through chunk and draw voxels
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            for (int j = 0; j < CHUNK_SIZE; j++)
            {
                for (int k = 0; k < CHUNK_SIZE; k++)
                {
                    if (m_Chunk->m_Voxels[i][j][k].m_Active)
                    {
                        glm::mat4 model = glm::translate(m_Model, glm::vec3(i, j, k));
                        m_Shader->SetUniformMat4f("u_Model", model);
                        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
                    }
                }
            }
        }
        // Draw
        // renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }
    
    void TestChunk::OnImGuiRender()
    {
        ImGui::Text("Press 'F' to toggle wireframe mode: %s", (m_wire_toggle ? "on" : "off"));
        ImGui::Text("Press 'C' to toggle mouse capture: %s", (m_mouse_captured ? "on" : "off"));
        ImGui::Text("Press 'V' to toggle all voxels active: %s", (m_Chunk->m_AllVoxelsActive ? "on" : "off"));
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