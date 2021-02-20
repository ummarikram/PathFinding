#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <list>
#include <queue>

int Get_Width() { return glfwGetVideoMode(glfwGetPrimaryMonitor())->width; }
int Get_Height() { return glfwGetVideoMode(glfwGetPrimaryMonitor())->height; }

#define Screen_Width Get_Width()
#define Screen_Height Get_Height()

#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Text-Rendering.h"
#include "HoverEffect.h"
#include "Shapes.h"
#include "Map.h"
#include "GLInit.h"

int main(int argc, char** argv)
{
    GLFWwindow* window;

    GLInit(window);

    // Initialize GLEW
    GLenum err = glewInit();

    // if error in GLEW
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    
    Map World;

    glfwSetMouseButtonCallback(window, World.mouseButtonCallBack);
    glfwSetCursorPosCallback(window, World.cursorPositionCallBack);
    glfwSetKeyCallback(window, World.KeyboardCallBack);

    // Shader reading
    Shader shader("Shaders/vs.shader", "Shaders/fs.shader");

    // Binding shader with text
    BindText(shader);

    // Loop until choice is selected OR window is not closed
    while (World.GetSelectedAlgorithm() == 0 && !glfwWindowShouldClose(window) && !QuitApp)
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // White Background
        glClearColor(1, 1, 1, 0);

        // To draw
        glLoadIdentity();
        glOrtho(0, 1280, 720, 0, 100, -100);

        // Store mouse positions
        World.SetCursorPosition(window);

        int HoverBlock = 0;
        bool isHovering = World.CheckBlockCollision(World.GetCursorPositionX() , World.GetCursorPositionY(), HoverBlock);

        // Play hover effect on rendered text if any.
        CheckHover(shader, HoverBlock, isHovering);
        World.InputAlgoChoice(HoverBlock);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        /* Poll for and process events */
        glfwPollEvents();

    }

    // Destroy window
    glfwDestroyWindow(window);

    if (World.GetSelectedAlgorithm() != 0 && !QuitApp)
    {
        World.ResetKeys();

        GLInit(window);

        glfwSetMouseButtonCallback(window, World.mouseButtonCallBack);
        glfwSetCursorPosCallback(window, World.cursorPositionCallBack);
        glfwSetKeyCallback(window, World.KeyboardCallBack);

        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window) && !QuitApp)
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            // White Background
            glClearColor(1, 1, 1, 0);

            // To draw
            glLoadIdentity();
            glOrtho(0, Screen_Width, Screen_Height, 0, 100, -100);

            World.DrawWorld();
            World.SetCursorPosition(window);
            World.UpdateMap(window);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}
