#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../include/GLFW/glfw3.h"

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float FOV         =  55.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    // Makes these variables global for other functions to access it
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    // euler Angles
    float Yaw;
    float Pitch;
    
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(2.0f, 5.0f, 10.0f),  // (left/right pos, up/down pos, front/back pos)
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
    float yaw = YAW, // intilize once
    float pitch = PITCH): // intilize once
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), // Front = glm::vec3 etc
    MovementSpeed(SPEED),   // MovementSpeed = SPEED
    MouseSensitivity(SENSITIVITY), // MouseSensitivity = SENSITIVITY
    Fov(FOV),    // Fov = FOV
    Yaw(yaw),
    Pitch(pitch),
    Position(position),
    WorldUp(up) {  
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 GetProjectionMatrix(float aspectRatio, float nearPlane = 0.1f, float farPlane = 100.0f) {
        return glm::perspective(glm::radians(Fov), aspectRatio, nearPlane, farPlane);
    }

    // Key control method using bool keys[1024];
    void keyControl(bool* keys, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (keys[GLFW_KEY_W]) {
            Position += Front * velocity;
        }
        if (keys[GLFW_KEY_S]) {
            Position -= Front * velocity;
        }
        if (keys[GLFW_KEY_A]) {
            Position -= Right * velocity;
        }
        if (keys[GLFW_KEY_D]) {
            Position += Right * velocity;
        }

        if (keys[GLFW_KEY_SPACE]) {
            Position += Up * velocity;
        }
            if (keys[GLFW_KEY_LEFT_ALT]) {
            Position -= Up * velocity;
        }
        
        // Shift key as sprint key
        float speed = 2.5f; // base speed
        MovementSpeed = keys[GLFW_KEY_LEFT_SHIFT] ? speed * 2.5f : speed;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void mouseControl(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void movelight(glm::vec3& lightPos) {
        float radius = 5.0f; // Radius of the circle
        float speed = 2.0f;   // Speed of light's movement
        float angle = glfwGetTime() * speed;

        lightPos.x = radius * cos(angle);  // Circular motion in the X direction
        lightPos.z = radius * sin(angle);  // Circular motion in the Z direction

        // Vertical oscillation (light moves up and down over time)
        lightPos.y = 2.0f + sin(glfwGetTime()) * 2.0f;  // Adds vertical movement
    }

    void movelight2(glm::vec3& lightPos2) { // move in different direction
        float radius = 5.0f; // Radius of the circle
        float speed = 1.0f;   // Speed of light's movement
        float angle = glfwGetTime() * speed;

        lightPos2.y = radius * cos(angle);  // Circular motion in the X direction
        lightPos2.z = radius * sin(angle);  // Circular motion in the Z direction

        // Vertical oscillation (light moves up and down over time)
        lightPos2.x = 2.0f + sin(glfwGetTime()) * 2.0f;  // Adds vertical movement
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif