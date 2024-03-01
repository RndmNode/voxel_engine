// #include <GL/glew.h>

#include "test_template.h"
#include "../renderer.h"
#include "../vendor/imgui/imgui.h"

namespace test {
    test_template::test_template()
    {
    }
    
    test_template::~test_template()
    {
    }
    
    void test_template::OnUpdate(float deltaTime)
    {
    }
    
    void test_template::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    
    void test_template::OnImGuiRender()
    {
        
    }
}