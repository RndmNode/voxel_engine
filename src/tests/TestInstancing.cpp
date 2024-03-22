
#include "TestInstancing.h"

namespace test {
    TestInstancing::TestInstancing()
    {
    }
    
    TestInstancing::~TestInstancing()
    {
    }
    
    void TestInstancing::OnUpdate(GLFWwindow *window, float deltaTime)
    {
    }
    
    void TestInstancing::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    
    void TestInstancing::OnImGuiRender()
    {
        
    }
    
    void TestInstancing::ProcessInput(GLFWwindow *window)
    {
        
    }
}