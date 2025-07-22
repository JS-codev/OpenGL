#include <iostream>
#include "window.h"
#include "shader.h"
#include "object.h"
#include "camera.h"

Camera camera; //declare here as GLOABL 

int main() {

    //1. Initilize Window
    GLFWwindow* window = initWindow(1200, 800, "OpenGL Triangle");
    if (!window) return -1;

    //2. Shapes vertex definitions (triangle, rectangle, etc) & shader compliation
    Tri tri;
    Shader shader("asset/shaders/vertex.vs","asset/shaders/fragment.fs");
    Shader light("asset/shaders/lightver.vs","asset/shaders/lightfrag.fs");
    glEnable(GL_DEPTH_TEST);
    // 3. Render loop
    while (!glfwWindowShouldClose(window)) {
        // register any key press:
        processInput(window);
        
        // Set background clear color to a dark teal
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the color buffer to apply above color theme

        //draw
        tri.draw(light, shader, camera); 

        // Swap front and back buffers to show updated drawn screen
        glfwSwapBuffers(window);

        //Check for any key press (GLFW_KEY_W) and let processInput(window) do the action
        glfwPollEvents();
    }

    // 4. De-allocate all resources
    std::cerr << "Freeing up memory...";
    tri.del();
    glDeleteProgram(shader.ID);
    glfwTerminate();
    return 0;
}