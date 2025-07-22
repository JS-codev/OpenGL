#include "headers/window.h"
#include <iostream>

Window::Window() {
    for (int i = 0; i < 1024; i++) {
        keys[i] = false;
    }

    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;

    for (int i = 0; i < 1024; i++) {
        keys[i] = false;
    }

    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
}

int Window::Initialise() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!\n";
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For Mac compatibility

    // Create window
    mainWindow = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
    if (!mainWindow) {
        std::cerr << "GLFW window creation failed!\n";
        glfwTerminate();
        return 1;
    }

    // Get buffer size info
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Make context current
    glfwMakeContextCurrent(mainWindow);

    // Handle user input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // Load OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return 1;
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    return 0;
}

void Window::createCallbacks() {
    glfwSetWindowUserPointer(mainWindow, this); // allows `->` to be use inside function
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}
GLfloat Window::getXChange() {
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange; // new mouse precision 
}

GLfloat Window::getYChange() {
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange; // new mouse precision   
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            theWindow->keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFirstMoved) {
        theWindow->lastX = static_cast<GLfloat>(xPos);
        theWindow->lastY = static_cast<GLfloat>(yPos);
        theWindow->mouseFirstMoved = false;
    }

    theWindow->xChange = static_cast<GLfloat>(xPos) - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - static_cast<GLfloat>(yPos);

    theWindow->lastX = static_cast<GLfloat>(xPos);
    theWindow->lastY = static_cast<GLfloat>(yPos);
}


Window::~Window() { // called this automatically when exiting main()
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}