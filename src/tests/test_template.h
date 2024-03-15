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
        void ProcessInput(GLFWwindow *window) override;
    private:
        
    };
} // namespace test
