#pragma once

#include "Test.h"

#include "../renderer.h"
#include "../vendor/imgui/imgui.h"

namespace test
{
    class TestInstancing : public Test {
    public:
        TestInstancing();
        ~TestInstancing();

        void OnUpdate(GLFWwindow *window, float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ProcessInput(GLFWwindow *window) override;
    private:
        
    };
} // namespace test
