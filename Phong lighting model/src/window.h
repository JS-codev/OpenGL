#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Initialize GLFW, create window, set up GLAD
GLFWwindow* initWindow(int width, int height, const char* title);

// Handles window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Handles input (like ESC key to close)
void processInput(GLFWwindow* window);

// Mouse navigation + scroll wheel 
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


#endif


