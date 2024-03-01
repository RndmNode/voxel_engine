#pragma once

#include "Test.h"

namespace test
{
    class test_template : public Test {
    public:
        test_template();
        ~test_template();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        
    };
} // namespace test
