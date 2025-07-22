#ifndef OBJECT_H
#define OBJECT_H

#define STB_IMAGE_IMPLEMENTATION 
#include "config.h"
#include "shader.h"
#include "camera.h"
#include <math.h>

void setupMesh(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,
               float* vertices, size_t size, unsigned int* indices, size_t indexSize, unsigned int &lightVAO);

class Tri {
private:
    unsigned int TVBO, TVAO, EBO, lightVAO;
    float cameraDistance = 3.0f;  // How far away from triangle
    float cameraAngle = 0.0f;     // Which direction around the triangle
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

public:

    Tri() {
        float vertices[] = { 
            // Position           // Normal vectors
            // Back face
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            // Front face
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            // Left face
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            // Right face
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            // Bottom face
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

            // Top face
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
        };

        unsigned int indices[] = {  // Cube has this many side faces
            // Back face
            0, 1, 2,  2, 3, 0,
            // Front face
            4, 5, 6,  6, 7, 4,
            // Left face
            8, 9, 10, 10,11,8,
            // Right face
            12,13,14, 14,15,12,
            // Bottom face
            16,17,18, 18,19,16,
            // Top face
            20,21,22, 22,23,20
        };
        
        setupMesh(TVAO, TVBO, EBO, vertices, sizeof(vertices), indices, sizeof(indices), lightVAO); 
}

    void draw(Shader &light, Shader &shader, Camera &camera) { 
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        camera.movelight(lightPos); 
        //Set the Coordinates matrix first before drawing:
        glm::mat4 view = camera.GetViewMatrix(); // initlize
        glm::mat4 projection = camera.GetProjectionMatrix(1200.0f / 800.0f);
   

        glm::vec3 cubePositions[] = { // diff locations
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 1.5f, -2.2f, -2.5f),
            glm::vec3( -1.7f, 2.0f, 2.0f),
            glm::vec3( 2.5f,  1.2f, 1.5f),
            glm::vec3(-1.3f,  3.0f, -1.5f)
        };

        // Draw the main colored object FIRST
        shader.use();  // Use the main shader for colored object
        shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f); // orange color   
        shader.setVec3("lightColor",  1.0f, 1.0f, 1.0f); // bright light
        shader.setVec3("viewPos", camera.Position); // view position (specular)
        shader.setVec3("lightPos", lightPos);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);


        //draw single model:
        for (unsigned i = 0; i < 5; i ++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            // float angle = glfwGetTime() * 20.0f + 25.0f; // rotate over time
            // model = glm::rotate(model, glm::radians(angle), lightPos);
            shader.setMat4("model",model);
            glBindVertexArray(TVAO);
            glDrawElements(GL_TRIANGLES,36, GL_UNSIGNED_INT, 0);
        }
        // Draw light cube SECOND
        light.use();   // Use the light shader for light source
        light.setMat4("view", view);
        light.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.9f)); // smaller cube
        light.setMat4("model", model);  
        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    
    void del() {
        glDeleteVertexArrays(1, &TVAO);
        glDeleteBuffers(1, &TVBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &lightVAO);
    }
};

void setupMesh(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,  
               float* vertices, size_t size, unsigned int* indices, size_t indexSize, unsigned int &lightVAO) {
    
    // make a "storage box"
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // open the book (where to read vertex data)
    glBindVertexArray(VAO);  
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // find where to read VAO data

    // Put triange points into this box
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); // call this once

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //knows how to index into the vertex buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW); // send into this box instead (call this once)

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 2. Setup light object VAO (use same VBO/EBO)
    glGenVertexArrays(1, &lightVAO); // open another book
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // open this to read light vertex data. 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Open this to read light vertex data. 

    // Only position attribute for light (no normals needed)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

#endif