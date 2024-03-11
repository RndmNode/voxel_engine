// #include <GL/glew.h>

#include "TestVoxel.h"
#include "../renderer.h"
#include "../vertex_buffer_layout.h"

#include "../vendor/imgui/imgui.h"
#include "GLFW/glfw3.h"
bool once = false;
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

// convert degrees to radians and vice versa
#define degreesToRadians(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define radiansToDegrees(angleRadians) ((angleRadians) * 180.0 / M_PI)

float FOV = 90.0f;                                          // Field of View
float TanHalfFOV = glm::tan(degreesToRadians(FOV / 2.0f));      // tangent of half the field of view
float d = 1 / TanHalfFOV;                                   // reciprocal of tangent

// glm::mat4 proj = glm::mat4(
//             1.0f, 0.0f, 0.0f, 0.0f,
//             0.0f, 1.0f, 0.0f, 0.0f,
//             0.0f, 0.0f, 1.0f, 0.0f,
//             0.0f, 0.0f, 0.0f, 1.0f
//         );

namespace test {
    TestVoxel::TestVoxel()
        : m_Voxel(0.0f, 0.0f, 0.0f, 1.0f)
        //   m_Projection(glm::perspective(glm::radians(90.0f), (float)900/(float)900, 0.1f, 100.0f))       // projection matrix
        //   m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))      // view matrix
    {

        // m_View = glm::mat4(
        //     1.0f, 0.0f, 0.0f, 0.0f,
        //     0.0f, 1.0f, 0.0f, 0.0f,
        //     0.0f, 0.0f, 1.0f, 0.0f,
        //     0.0f, 0.0f, 0.0f, 1.0f
        // );

        // Build layout for vertex buffer
        VertexBufferLayout layout;
        layout.Push<float>(3);

        // Set vertex buffer
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(glm::vec3) * 8);
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        // Set Index buffer
        m_IndexBuffer = std::make_unique<IndexBuffer>(voxel_indices, 36);

        // Set Shader
        m_Shader = std::make_unique<Shader>("res/shaders/simple.shader");
        m_Shader->Bind();

        // Enable Depth Buffer
        GLCall(glEnable(GL_DEPTH_TEST));

        // Enable Face Culling
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        GLCall(glFrontFace(GL_CW));
    }
    
    TestVoxel::~TestVoxel()
    {
    }
    
    void TestVoxel::OnUpdate(float deltaTime)
    {
    }
    
    void TestVoxel::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // Get vertices from voxel
        glm::vec3 vertices[8];
        memcpy(vertices, m_Voxel.m_Vertices.data(), sizeof(vertices[0]) * 8);

        // glm::mat4 model = glm::mat4(1.0f);
        // glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(75.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 model = glm::mat4(1.0);

        if (m_rotate_toggle)
        {
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(75.0f), glm::vec3(0.5f, 1.0f, 0.2f));
        } else {
            model = glm::mat4(1.0);
        }

        if (m_proj_toggle)
        {
            m_Projection = glm::perspective(glm::radians(FOV), (float)900/(float)900, 0.1f, 100.0f);
        } else {
            m_Projection = glm::mat4(1.0);
        }

        if (m_view_toggle)
        {
            m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        } else {
            m_View = glm::mat4(1.0);
        }

        // instantiate renderer
        Renderer renderer;

        // Set Dynamic Vertex Buffer
        m_VertexBuffer->Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

        // Send MVP matrix to Shader
        // glm::mat4 mvp = m_Projection * m_View * model;
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniformMat4f("u_View", m_View);
        m_Shader->SetUniformMat4f("u_Projection", m_Projection);

        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }
    
    void TestVoxel::OnImGuiRender()
    {
        ImGui::Checkbox("Rotate", &m_rotate_toggle);
        ImGui::Checkbox("Projection with GLM", &m_proj_toggle);
        if (m_proj_toggle)
        {
            ImGui::SliderFloat("Field of View", &FOV, 0.0f, 180.0f);

        }
        ImGui::Checkbox("View with GLM", &m_view_toggle);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // framerate

    }
}