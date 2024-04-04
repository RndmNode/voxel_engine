
#include "test_template.h"

namespace test {
    test_template::test_template()
    {
    }
    
    test_template::~test_template()
    {
    }
    
    void test_template::OnUpdate(GLFWwindow *window, float deltaTime)
    {
    }
    
    void test_template::OnRender()
    {
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    
    void test_template::OnImGuiRender()
    {
        
    }
    
    void test_template::SetMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        
    }

    void test_template::UpdatePlayerPosition(glm::vec3 position)
    {
        
    }

    
}