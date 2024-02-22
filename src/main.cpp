#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include "renderer.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "vertex_buffer_layout.h"
#include "shader.h"
#include "Texture.h"

// Resize window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // init GLEW
    // ---------
    if(glewInit()!= GLEW_OK)
        std::cout << "Could not init GLEW" << std::endl;

    // Create scope to limit lifetime of stack allocated buffers
    {
        // Triangle
        // --------
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
            0.5f, -0.5f,  1.0f, 0.0f, // 1
            0.5f,  0.5f,  1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 1.0f, // 3
        };
        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Enable blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Buffers
        // -------
        // Vertex Buffer
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        // Index Buffer
        IndexBuffer ib(indicies, 6);

        // Vertex Array
        VertexArray va;
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Shaders
        // -------
        Shader shader("res/shaders/test.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.5f, 0.2f, 0.7f, 1.0f);

        // Textures
        // --------
        std::string path = "res/textures/space.png";
        Texture texture(path);
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
        
        // reset buffers
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        // Renderer
        // --------
        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;
        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // input
            // -----
            processInput(window);

            /* Render here */
            renderer.Clear();

            // shader
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.2f, 0.7f, 1.0f);

            // draw
            renderer.Draw(va, ib, shader);

            // change red value of color that is fed into the uniform 'u_Color'
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}