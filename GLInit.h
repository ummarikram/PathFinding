#pragma once

int GLInit(GLFWwindow*& window)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(Screen_Width, Screen_Height, "Path Finding", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Start window at center
    glfwSetWindowPos(window, Screen_Width / 20, Screen_Height / 20);
}