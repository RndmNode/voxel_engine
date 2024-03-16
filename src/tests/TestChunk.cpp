
#include "TestChunk.h"

#include "../vertex_buffer_layout.h"
#include "../vendor/glm/gtc/matrix_transform.hpp"

unsigned int const voxel_indices[36] = {
    // 0, 1, 2, 2, 3, 0, // front
    // 4, 5, 6, 6, 7, 4, // back
    // 0, 4, 7, 7, 3, 0, // left
    // 1, 5, 6, 6, 2, 1, // right
    // 0, 1, 5, 5, 4, 0, // top
    // 3, 2, 6, 6, 7, 3  // bottom
    0, 1, 2,
    1, 3, 4,
    5, 6, 3,
    7, 3, 6,
    2, 4, 7,
    0, 7, 6,
    0, 5, 1,
    1, 5, 3,
    5, 0, 6,
    7, 4, 3,
    2, 1, 4,
    0, 2, 7
};

namespace test {
    TestChunk::TestChunk()
        : m_Voxel(0.0f, 0.0f, 0.0f, 1.0f)
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
        
    }
    
    TestChunk::~TestChunk()
    {
    }
    
    void TestChunk::OnUpdate(GLFWwindow *window, float deltaTime)
    {
        // Movement speed
        float cameraSpeed = 2.5f * deltaTime;

        // WASD Camera Movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_Cam_Pos += cameraSpeed * m_CameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_Cam_Pos -= cameraSpeed * m_CameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_Cam_Pos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_Cam_Pos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
    }
    
    void TestChunk::OnRender()
    {
        // Clear the screen
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // Model transformation
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));     // Model translation
        model = glm::scale(model, glm::vec3(1.0));         // Model scale
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.2f));

        // Camera
        m_Projection = glm::perspective(glm::radians(m_FOV), (float)900/(float)900, 0.1f, 100.0f);
        m_View = glm::lookAt(m_Cam_Pos, m_Cam_Pos + m_CameraFront, m_CameraUp);       // Free Camera (no pitch or yaw)

        // Instantiate renderer
        Renderer renderer;
        m_Texture->Bind(0);

        // Set Dynamic Vertex Buffer
        m_VertexBuffer->Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 5 * 8, m_Voxel.m_Vertices));

        // Send MVP matrices to Shader
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniformMat4f("u_View", m_View);
        m_Shader->SetUniformMat4f("u_Projection", m_Projection);

        // Draw
        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }
    
    void TestChunk::OnImGuiRender()
    {
        ImGui::Checkbox("Wireframe", &m_wire_toggle);
        if (m_wire_toggle)
        {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
        } else {
            GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        }
        
        ImGui::Text("\nApplication average %.3f ms/frame (%.1f FPS)", 
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // framerate
    }
    
    void TestChunk::ProcessInput(GLFWwindow *window)
    {
        
    }
}