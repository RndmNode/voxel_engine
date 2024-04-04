#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "../renderer.h"

namespace test {

    class Test 
    {
    public:
        Test() {}
        virtual ~Test() {}

        virtual void OnUpdate(GLFWwindow *window, float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
        virtual void ProcessInput(GLFWwindow *window) {}
        virtual void SetMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {}
        virtual void UpdatePlayerPosition(glm::vec3 position) {}

    private:
        std::unique_ptr<Shader> m_Shader;
    };

    class TestMenu : public Test 
    {
    public:
        TestMenu(Test*& currentTestPointer);
        ~TestMenu() {};

        void OnImGuiRender() override;

        template <typename T>
        void RegisterTest(const std::string& name)
        {
            std::cout << "Registering test " << name << "\n";
            m_Tests.push_back(std::make_pair(name, []() {return new T();}));
        }
    private:
        Test*& m_CurrentTest;
        std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;

    };

}