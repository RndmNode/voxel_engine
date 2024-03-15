// #include <GL/glew.h>

#include "TestBatchTextures.h"
#include "../renderer.h"

struct Vertex 
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoords;
    float TexIndex;
};

static std::array<Vertex, 4> CreateQuad(float x, float y, float textureID)
{
    float size = 0.5f;

    Vertex v0;
    v0.Position = { x, y, 0.0f };
    v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v0.TexCoords = { 0.0f, 0.0f };
    v0.TexIndex = textureID;

    Vertex v1;
    v1.Position = { x + size, y, 0.0f };
    v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v1.TexCoords = { 1.0f, 0.0f };
    v1.TexIndex = textureID;

    Vertex v2;
    v2.Position = { x + size, y + size, 0.0f };
    v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v2.TexCoords = { 1.0f, 1.0f };
    v2.TexIndex = textureID;

    Vertex v3;
    v3.Position = { x, y + size, 0.0f };
    v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v3.TexCoords = { 0.0f, 1.0f };
    v3.TexIndex = textureID;

    return { v0, v1, v2, v3 };
};

namespace test {
    TestBatchTextures::TestBatchTextures()
        : m_Projection(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),       // projection matrix
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))     // view matrix
    {

        // Two Quads
        // --------
        /*float positions[] = {
          //  x      y      z          r     g     b      a   texture coords   Texture map
            -0.75f, -0.25f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,   0.0f, 0.0f,     0.0f, // 0
            -0.25f, -0.25f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,   1.0f, 0.0f,     0.0f, // 1
            -0.25f,  0.25f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,   1.0f, 1.0f,     0.0f, // 2
            -0.75f,  0.25f, 0.0f,   0.18f, 0.6f, 0.96f, 1.0f,   0.0f, 1.0f,     0.0f, // 3

             0.25f, -0.25f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,   0.0f, 0.0f,     1.0f, // 4
             0.75f, -0.25f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,   1.0f, 0.0f,     1.0f, // 5
             0.75f,  0.25f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,   1.0f, 1.0f,     1.0f, // 6
             0.25f,  0.25f, 0.0f,   1.0f, 0.93f, 0.24f, 1.0f,   0.0f, 1.0f,     1.0f  // 7
        };*/

        unsigned int indicies[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        // Build layout for vertex buffer
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(4);
        layout.Push<float>(2);
        layout.Push<float>(1);

        // Set vertex buffer
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 1000);
        m_VAO = std::make_unique<VertexArray>();
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        // Set Index buffer
        m_IBO = std::make_unique<IndexBuffer>(indicies, 12);

        // Set Shader
        m_Shader = std::make_unique<Shader>("res/shaders/tests/BatchTextures.shader");
        m_Shader->Bind();

        // Grab Textures
        std::string wood = "res/textures/Wood_Box.png";
        std::string leather = "res/textures/Leather.png";
        textures.push_back(std::make_unique<Texture>(wood));
        textures.push_back(std::make_unique<Texture>(leather)); 
        textures[0]->Bind(0);
        textures[1]->Bind(1);

        // Set texture uniforms
        int samplers[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, samplers);
    }
    
    TestBatchTextures::~TestBatchTextures()
    {
    }
    
    void TestBatchTextures::OnUpdate(GLFWwindow *window, float deltaTime)
    {
    }
    
    void TestBatchTextures::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        // Dynamic Quads
        auto q0 = CreateQuad(m_Quad_1_Position[0], m_Quad_1_Position[1], 0.0f);
        auto q1 = CreateQuad(0.25f, -0.25f, 1.0f);

        // Make buffer for quads
        Vertex vertices[8];
        memcpy(vertices, q0.data(), sizeof(Vertex) * q0.size());
        memcpy(vertices + q0.size(), q1.data(), sizeof(Vertex) * q1.size());

        // instantiate renderer
        Renderer renderer;

        // Set Dynamic Vertex Buffer
        m_VertexBuffer->Bind();
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
        
        // Draw
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
    }
    
    void TestBatchTextures::OnImGuiRender()
    {
        // Control first quad's position
        ImGui::DragFloat2("Quad 1 Position:", m_Quad_1_Position);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);  // framerate
        
    }
}