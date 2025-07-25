#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight); // get window's width, height and keys

    int Initialise(); // create window screen

    GLint getBufferWidth() { return bufferWidth; }
    GLint getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    bool* getsKeys() { return keys; } // WASD controls
    GLfloat getXChange();
    GLfloat getYChange();

    void swapBuffers() { glfwSwapBuffers(mainWindow); }

    ~Window();

private:
    GLFWwindow* mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;

    bool keys[1024];

    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFirstMoved;

    void createCallbacks();
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

#endif