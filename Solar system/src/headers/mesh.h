#ifndef MESH_H
#define MESH_H
#define STB_IMAGE_IMPLEMENTATION 
#include "config.h"
#include "shader.h"
#include "camera.h"
#include <math.h>
#define M_PI 3.14159265358979323846

void setupMesh(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,
               const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
               unsigned int &lightVAO,  unsigned int &backgroundVAO, unsigned int &backgroundVBO,  const float* quadVertices, size_t quadSizeBytes);

// Draw sphere
void createSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices,
                  unsigned int X_SEGMENTS = 128, unsigned int Y_SEGMENTS = 128, float radius = 1.0f) {
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            float xSegment = static_cast<float>(x) / X_SEGMENTS;
            float ySegment = static_cast<float>(y) / Y_SEGMENTS;

            float theta = ySegment * M_PI;
            float phi = xSegment * 2.0f * M_PI;

            float xPos = radius * std::sin(theta) * std::cos(phi);
            float yPos = radius * std::cos(theta);
            float zPos = radius * std::sin(theta) * std::sin(phi);

            // Position
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);
            
            // Normal (normalized position vector for unit sphere)
            float length = std::sqrt(xPos*xPos + yPos*yPos + zPos*zPos);
            vertices.push_back(xPos / length);
            vertices.push_back(yPos / length);
            vertices.push_back(zPos / length);
            
            // Texture coordinates
            vertices.push_back(xSegment);
            vertices.push_back(ySegment);
        }
    }

    // Generate indices
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x < X_SEGMENTS; ++x) {
            unsigned int i0 = y * (X_SEGMENTS + 1) + x;
            unsigned int i1 = (y + 1) * (X_SEGMENTS + 1) + x;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i0 + 1);

            indices.push_back(i0 + 1);
            indices.push_back(i1);
            indices.push_back(i1 + 1);
        }
    }
}

// texture loading function
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
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
    unsigned int VBO, VAO, EBO, lightVAO, backgroundVAO, backgroundVBO;
    float cameraDistance = 3.0f;  // How far away from triangle
    float cameraAngle = 0.0f;     // Which direction around the triangle
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
    size_t indexCount;

public:
    unsigned int earthDiffuseMap = loadTexture("asset/earth.png");    // Replace with your Earth texture path
    unsigned int  earthSpecularMap = loadTexture("asset/earth_specular.png"); 
    unsigned int sunTexture = loadTexture("asset/sun.png"); // Or PNG
    unsigned int backgroundTexture = loadTexture("asset/stars.png");

    Tri() {
        std::vector<float> sphereVertices;
        std::vector<unsigned int> sphereIndices;
        createSphere(sphereVertices, sphereIndices);
        indexCount = sphereIndices.size();  // store count for sphere

        // background 
        float quadVertices[24] = {
            // positions   // texCoords
            // X     Y     U     V
            -1.0,  1.0,  0.0, 1.0,  // Top-left
            -1.0, -1.0,  0.0, 0.0,  // Bottom-left
            1.0, -1.0,  1.0, 0.0,  // Bottom-right

            -1.0,  1.0,  0.0, 1.0,  // Top-left
            1.0, -1.0,  1.0, 0.0,  // Bottom-right
            1.0,  1.0,  1.0, 1.0   // Top-right
        };

        setupMesh(VAO, VBO, EBO, sphereVertices, sphereIndices, lightVAO, backgroundVAO, backgroundVBO, quadVertices, sizeof(quadVertices));

    }

void draw(Shader &light, Shader &shader, Shader &background, Camera &camera) { 
    //background
    background.use(); // use the simple shader you created
    glBindVertexArray(backgroundVAO);
    glDisable(GL_DEPTH_TEST); // background should not occlude anything
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    background.setInt("background", 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);

    glm::vec3 lightPos(1.2f, 1.0f, 0.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix(1200.0f / 800.0f);

    // Draw the main colored object FIRST
    shader.use();  // Use the main shader for colored object
    
    shader.setVec3("viewPos", camera.Position);
    shader.setVec3("lightPos", lightPos);  // Make sure this matches your fragment shader
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    // Bind Earth textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthDiffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, earthSpecularMap);
    
    // Set texture uniforms
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.setFloat("material.shininess", 32.0f);
    
    // Light properties 
    shader.setVec3("light.ambient", 0.25f, 0.25f, 0.25f);   // Reduced ambient for more dramatic lighting
    shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);   // Brighter diffuse
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // Draw sphere
    glm::mat4 model = glm::mat4(1.0f); 
    // Rotate around sun
    float angle = glfwGetTime() * 0.5f;
    glm::vec3 orbitCenter = lightPos; // move at sun position
    float x = orbitCenter.x + 5.4f * std::cos(angle);
    float z = orbitCenter.z + 5.0f * std::sin(angle);
    float y = orbitCenter.y + 1.5f * std::sin(glfwGetTime() * 1.0f);
 
    glm::vec3 earthPos = glm::vec3(x, y, z);
    model = glm::translate(model, earthPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(23.5f), glm::vec3(0.0f, 0.0f, 1.0f)); // Tilt like Earth's axis (23.5 degree) at Z-tilt
    model = glm::rotate(model, angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

    // Sun
    light.use();
    light.setMat4("view", view);
    light.setMat4("projection", projection);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(1.2f)); // Make it smaller
    light.setMat4("model", model);  
    // Bind sun texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    light.setInt("sunTexture", 0);
    glBindVertexArray(lightVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

}
    
    void del() {  // cal this by object.del();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &lightVAO);
        glDeleteTextures(1, &earthDiffuseMap);
        glDeleteTextures(1, &earthSpecularMap); 
        glDeleteVertexArrays(1, &backgroundVAO);
        glDeleteBuffers(1, &backgroundVBO);
        glDeleteTextures(1, &backgroundTexture);
    }
};
 
void setupMesh(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,
               const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
               unsigned int &lightVAO, unsigned int &backgroundVAO, unsigned int &backgroundVBO,  const float* quadVertices, size_t quadSizeBytes) {
        // Generate buffers
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);  

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Texture coordinate attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Setup light VAO using same VBO and EBO
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // sun vertices

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1); // sun texture
    
    // Set up background
    glGenVertexArrays(1, &backgroundVAO);
    glGenBuffers(1, &backgroundVBO);
    glBindVertexArray(backgroundVAO);

    glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, quadSizeBytes, quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
#endif