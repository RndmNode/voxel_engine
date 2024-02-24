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

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

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
    int w_width  = 960;
    int w_height = 540;
    window = glfwCreateWindow(w_width, w_height, "Test", NULL, NULL);
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
            100.0f, 100.0f, 0.0f, 0.0f, // 0
            200.0f, 100.0f, 1.0f, 0.0f, // 1
            200.0f, 200.0f, 1.0f, 1.0f, // 2
            100.0f, 200.0f, 0.0f, 1.0f, // 3
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

        // Matricies
        // ---------
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));                 // Model Matrix
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));                   // View Matrix
        glm::mat4 projection = glm::ortho(0.0f, float(w_width), 0.0f, float(w_height), -1.0f, 1.0f);        // Projection Matrix

        glm::mat4 mvp = projection * view * model;

        // Shaders
        // -------
        Shader shader("res/shaders/test.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.5f, 0.2f, 0.7f, 1.0f); // Set Color
        shader.SetUniformMat4f("u_MVP", mvp); // Set projection matrix

        // Textures
        // --------
        std::string path = "res/textures/space.png";
        Texture texture(path);
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0); // Set Texture
        
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
            // shader.SetUniform4f("u_Color", r, 0.2f, 0.7f, 1.0f);
            // shader.SetUniform1i("u_Texture", 0);

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