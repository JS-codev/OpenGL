#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "headers/window.h"
#include "headers/shader.h"
#include "headers/mesh.h"
#include "headers/camera.h"

// Global variables
Window mainWindow; // create object and run Window();
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// User input
void userinput() {
        camera.keyControl(mainWindow.getsKeys(), deltaTime); // getKeys() returns bool keys[1024];
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
}

int main() {
    // 1. Initialize Window using the new Window class
    mainWindow = Window(1200, 800);
    if (mainWindow.Initialise() != 0) {
        return -1;
    }

    // 2. Create objects and shaders
    Tri tri;
    Shader shader("asset/shaders/vertex.vs","asset/shaders/fragment.fs");
    Shader light("asset/shaders/lightver.vs","asset/shaders/lightfrag.fs");

    // 3. Initialize camera and shader
    camera = Camera();
  
    // 4. Render loop
    while (!mainWindow.getShouldClose()) {
        // Calculate delta time
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // updates key states and mouse changes after reading userinput();
        glfwPollEvents();  // call this first before userinput();

        // read / process each user inputs
        userinput(); 

        // Set background clear color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw 
        tri.draw(light, shader, camera);

        // Swap buffers
        mainWindow.swapBuffers();
        glfwSwapInterval(0); // Disable VSync
    }

    // 5. Cleanup
    std::cerr << "Freeing up memory...\n";
    tri.del();
    glDeleteProgram(shader.ID);
    glDeleteProgram(light.ID);

    return 0;
}