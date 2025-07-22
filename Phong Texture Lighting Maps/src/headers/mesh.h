#ifndef MESH_H
#define MESH_H
#define STB_IMAGE_IMPLEMENTATION 
#include "config.h"
#include "shader.h"
#include "camera.h"
#include <math.h>
void setupMesh(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,
               float* vertices, size_t size, unsigned int* indices, size_t indexSize, unsigned int &lightVAO);

  // texture
    unsigned int loadTexture(char const * path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

class Tri {
private:
    unsigned int VBO, VAO, EBO, lightVAO;
    float cameraDistance = 3.0f;  // How far away from triangle
    float cameraAngle = 0.0f;     // Which direction around the triangle
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

public:
    // Set up texture:
    unsigned int diffuseMap = loadTexture("asset/box.png");
    unsigned int specularMap = loadTexture("asset/box_specular.png");

  

    Tri() {
        float vertices[] = { 
        // Position           // Normal vectors    // texture
       // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // Right face
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
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

        setupMesh(VAO, VBO, EBO, vertices, sizeof(vertices), indices, sizeof(indices), lightVAO); 
}

    void draw(Shader &light, Shader &shader, Camera &camera) { 

        // Light source starting positions
        glm::vec3 lightPos(1.2f, 1.0f, -2.0f);
        glm::vec3 lightPos2(-10.0f, 1.0f, 2.0f);

        // enable this to move sunlight over time
        camera.movelight(lightPos); 
        camera.movelight2(lightPos2);
        glm::mat4 view = camera.GetViewMatrix(); // initlize
        glm::mat4 projection = camera.GetProjectionMatrix(1200.0f / 800.0f);
   
        glm::vec3 cubePositions[] = { // diff locations
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 1.5f, -2.2f, -2.5f),
            glm::vec3( -1.7f, 2.0f, 2.0f),
            glm::vec3( 2.5f,  1.2f, 1.5f),
            glm::vec3(3.4f,  3.0f, -1.5f)
        };

        // Draw the main colored object FIRST
        // define shine output and reflection output
        shader.use();  // Use the main shader for colored object
         // texture
        shader.setInt("material.diffuse", 0); // texture
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", 64.0f); // higher = sharper shine spot

        shader.setVec3("viewPos", camera.Position); // view position (specular)
        shader.setVec3("light.position", lightPos);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
   
        // light structure 
        shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f); 
        shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        shader.setVec3("light.specular", 1.5f, 1.5f, 1.5f);

        //draw single model:
        for (unsigned i = 0; i < 5; i ++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = glfwGetTime() * 20.0f + 25.0f; // rotate over time
            //model = glm::rotate(model, glm::radians(angle), lightPos);
            shader.setMat4("model",model);
            // Texture:
            glActiveTexture(GL_TEXTURE0); 
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            glActiveTexture(GL_TEXTURE1); 
            glBindTexture(GL_TEXTURE_2D, specularMap);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES,36, GL_UNSIGNED_INT, 0);
        }

        // Draw light cube SECOND
        light.use();   // Use the light shader for light source
        light.setMat4("view", view);
        light.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(1.1f)); // smaller cube
        light.setMat4("model", model);  

        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // second light source
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos2);
        model = glm::scale(model, glm::vec3(1.1f)); // smaller cube
        light.setMat4("model", model);  
        
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    
    void del() {  // cal this by object.del();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 2. Setup light object VAO (use same VBO/EBO)
    glGenVertexArrays(1, &lightVAO); // open another book
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // open this to read light vertex data. 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Open this to read light vertex data. 
        
    // Only position attribute for light (no normals needed)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
#endif