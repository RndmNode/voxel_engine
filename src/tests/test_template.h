#pragma once

#include "Test.h"

#include "../renderer.h"
#include "../vendor/imgui/imgui.h"

namespace test
{
    class test_template : public Test {
    public:
        test_template();
        ~test_template();

        void OnUpdate(GLFWwindow *window, float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void SetMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection) override;
        void UpdatePlayerPosition(glm::vec3 position) override;
    private:
        
    };
} // namespace test
