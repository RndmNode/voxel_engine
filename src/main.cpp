// #include <iostream>
// #include <fstream>
// #include <string>
// #include <sstream>
// #include <iomanip>

#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include "shader.h"
#include "Texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestBatchColorQuads.h"
#include "tests/TestBatchTextures.h"
#include "tests/TestVoxel.h"

// #include <GL/glew.h>
// #include <GLFW/glfw3.h>

/*
    This is a rudimentary voxel engine.
    It utilizes GLFW for OpenGL window rendering and input processing.
    It employs GLEW to be able to call all of the OpenGL functions.
    GLM is used for math.
    ImGui is used for being able to test and set variables while the executable is running.

    This was developed with the intention of experimenting with 3D graphics and OpenGL
        as well as learning strategies for optimizing performance.
*/

// Resize window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main(void)
{
    // Instantiate GLFW Window
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // initialize settings for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create a windowed mode window and its OpenGL context
    int w_width  = 900;
    int w_height = 900;
    window = glfwCreateWindow(w_width, w_height, "Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    // init GLEW
    // ---------
    if(glewInit()!= GLEW_OK)
        std::cout << "Could not init GLEW" << std::endl;

    // Create scope to limit lifetime of stack allocated buffers
    {
        // Renderer
        // --------
        Renderer renderer;

        // instantiate tests
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        // Add tests to executable
        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
        testMenu->RegisterTest<test::TestBatchColorQuads>("Batch Rendering");
        testMenu->RegisterTest<test::TestBatchTextures>("Batch Textures");
        testMenu->RegisterTest<test::TestVoxel>("Voxel");

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // time
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;  

            // input
            // -----
            processInput(window);
            // currentTest->ProcessInput(window);

            /* Render here */
            renderer.Clear();

            // imgui start new frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Test framework
            if (currentTest)
            {
                // Check if a test has been selected to run and render if so
                currentTest->OnUpdate(window, deltaTime);
                currentTest->OnRender();
                ImGui::Begin("Test");

                // If in a test and we pressed the 'back' button
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    // exit the test
                    delete currentTest;
                    currentTest = testMenu;
                }

                // Render
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            // imgui rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // Cleanup tests
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    GLCall(glViewport(0, 0, width, height));
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // Close application if 'Esc' is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}