// #include <GL/glew.h>

#include "TestVoxel.h"
#include "../renderer.h"
#include "../vertex_buffer_layout.h"

#include "../vendor/imgui/imgui.h"
#include "GLFW/glfw3.h"

unsigned int const voxel_indices[36] = {
    0, 1, 2, 2, 3, 0, // front
    4, 5, 6, 6, 7, 4, // back
    0, 4, 7, 7, 3, 0, // left
    1, 5, 6, 6, 2, 1, // right
    0, 1, 5, 5, 4, 0, // top
    3, 2, 6, 6, 7, 3  // bottom
};

// std::array<Vertex, 4> CreateVoxel(Voxel voxel)
// {
//     float size = voxel.getSize();
//     float x = voxel.getX();
//     float y = voxel.getY();
//     float z = voxel.getZ();

//     Vertex v0;
//     v0.Position = { x, y, z };
//     v0.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

//     Vertex v1;
//     v1.Position = { x + size, y, z };
//     v1.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

//     Vertex v2;
//     v2.Position = { x + size, y + size, z };
//     v2.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

//     Vertex v3;
//     v3.Position = { x, y + size, z };
//     v3.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

//     return { v0, v1, v2, v3, };
// }

namespace test {
    TestVoxel::TestVoxel()
        : m_Voxel(-0.5f, -0.5f, 0.0f, 1.0f),
          m_Projection(glm::ortho(0.0f, 1200.0f, 0.0f, 900.0f, -1.0f, 1.0f)),       // projection matrix
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))      // view matrix
    {
        // Build layout for vertex buffer
        VertexBufferLayout layout;
        layout.Push<float>(3);

        // Set vertex buffer
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(glm::vec3) * 8);
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        // for (auto& i : voxel_indices)
        // {
        //     std::cout << i << std::endl;
        // }

        // Set Index buffer
        m_IndexBuffer = std::make_unique<IndexBuffer>(voxel_indices, 36);

        // Set Shader
        m_Shader = std::make_unique<Shader>("res/shaders/simple.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
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
        GLCall(glClear(GL_COLOR_BUFFER_BIT));


        // FIGURE OUT HOW TO GIVE VOXEL DATA TO VERTEX BUFFER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        glm::vec3 vertices[8];
        memcpy(vertices, m_Voxel.m_Vertices.data(), sizeof(vertices[0]) * 8);

        // glm::mat4 model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        // glm::mat4 model = glm::mat4(1.0f);

        // instantiate renderer
        Renderer renderer;

        // Set Dynamic Vertex Buffer
        m_VertexBuffer->Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

        // glm::mat4 mvp = m_Projection * m_View * model;
        // m_Shader->Bind();
        // m_Shader->SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }
    
    void TestVoxel::OnImGuiRender()
    {
        
    }
}