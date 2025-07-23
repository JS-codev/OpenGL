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
               unsigned int &lightVAO, unsigned int &backgroundVAO, unsigned int &backgroundVBO,  const float* quadVertices, size_t quadSizeBytes);

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
    unsigned int VBO, VAO, EBO, lightVAO, moonVAO, backgroundVAO, backgroundVBO;
    float cameraDistance = 3.0f;  // How far away from triangle
    float cameraAngle = 0.0f;     // Which direction around the triangle
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
    size_t indexCount;

public:
    unsigned int earthDiffuseMap = loadTexture("asset/textures/earth.png");    // Replace with your Earth texture path
    unsigned int earthSpecularMap = loadTexture("asset/textures/earth_specular.png"); 
    unsigned int sunTexture = loadTexture("asset/textures/sun.png"); // Or PNG
    unsigned int backgroundTexture = loadTexture("asset/textures/stars.png");
    unsigned int moonTexture = loadTexture("asset/textures/moon.png");
    unsigned int mercury = loadTexture("asset/textures/mercury.png");
    unsigned int mars = loadTexture("asset/textures/mars.png");
    unsigned int venus = loadTexture("asset/textures/venus.png");
    unsigned int uranus = loadTexture("asset/textures/uranus.png");
    unsigned int neptune = loadTexture("asset/textures/neptune.png");
    unsigned int saturn = loadTexture("asset/textures/saturn.png");
    unsigned int saturnRing = loadTexture("asset/textures/saturn_ring.png");
    unsigned int jupiter = loadTexture("asset/textures/jupiter.png");

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

    float timeScale = 0.01f; // Slow down time for better visual (or else all planets rotate too fast to preview)
    float sizeScale = 10.0f; // increase planet size for better visual
    
    // Sun
    float ssize = 12.8f; // Sun size
    light.use(); // light shader for sun
    light.setMat4("view", view);
    light.setMat4("projection", projection);
    float angle = glfwGetTime() * 0.12f; 
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::rotate(model, glm::radians(7.25f), glm::vec3(0.0f, 0.0f, 1.0f)); // Sun axial tilt
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f)); // moves at axial tilt direction 
    model = glm::scale(model, glm::vec3(ssize)); 
    light.setMat4("model", model);  
    // Bind sun texture & draw
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    light.setInt("sunTexture", 0);
    glBindVertexArray(lightVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

    shader.use();  // Use the main shader for colored object (earth, moon, etc)
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

    // Earth
    model = glm::mat4(1.0f); 
    angle = glfwGetTime() * 29.78f * timeScale; // how fast it moves around sun. 
    glm::vec3 orbitCenter = lightPos; // move to Sun position
    float x = (orbitCenter.x + ssize + 1.0f * sizeScale) * std::cos(angle); // x,y,z moves at sun's diameter position, then + 100f from the sun
    float z = (orbitCenter.z + ssize + 1.0f * sizeScale) * std::sin(angle);
    float y = (orbitCenter.y + 3.5f) * std::sin(angle);
    glm::vec3 earthPos = glm::vec3(x, y, z);
    model = glm::translate(model, earthPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(23.5f), glm::vec3(0.0f, 0.0f, 1.0f)); // Tilt like Earth's axis (23.5 degree) at Z-tilt
    model = glm::rotate(model, angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

    // Moon
    model = glm::mat4(1.0f); 
    angle = glfwGetTime() * 1.022f; // moves slightly faster than earth 
    orbitCenter = earthPos; // move to Earth postiion
    float mx = orbitCenter.x + 1.2f * std::cos(-angle);  // Negative sign for opposite direction
    float mz = orbitCenter.z + 1.2f * std::sin(-angle);
    float my = orbitCenter.y + 0.75f * std::sin(glfwGetTime() * 0.5f);
    glm::vec3 moonPos = glm::vec3(mx, my, mz);
    model = glm::translate(model, moonPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(5.1f), glm::vec3(0.0f, 0.0f, 1.0f)); // Moon's axial tilt
    model = glm::rotate(model, angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)); // Moon's rotation
    model = glm::scale(model, glm::vec3(0.204f)); // one-quarter the diameter of Earth
    shader.setMat4("model", model);
    // bind moon texture & draw
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    shader.setInt("moonTexture", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

    // Mercury 
    model = glm::mat4(1.0f); 
    angle = glfwGetTime() * 122.8f * timeScale; // nearest to sun = fastest rotation
    orbitCenter = lightPos; // move to Earth postiion
    float mex = (orbitCenter.x + ssize + 0.39f * sizeScale) * std::cos(angle);
    float mez = (orbitCenter.z + ssize + 0.39f * sizeScale) * std::sin(angle);
    float mey = (orbitCenter.y + 2.5f) * std::sin(angle);
    glm::vec3 merPos = glm::vec3(mex, mey, mez);
    model = glm::translate(model, merPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(0.034f), glm::vec3(0.0f, 0.0f, 1.0f)); // Mercury axial tilt
    model = glm::rotate(model, angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)); 
    model = glm::scale(model, glm::vec3(0.287f)); 
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mercury);
    shader.setInt("mercury", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
    
    // Venus
    model = glm::mat4(1.0f); 
    angle = glfwGetTime() * 48.6f * timeScale; 
    orbitCenter = lightPos; // move to Earth postiion
    float vx = (orbitCenter.x + ssize + 0.72f * sizeScale) * std::cos(angle);
    float vz = (orbitCenter.z + ssize + 0.72f * sizeScale) * std::sin(angle);
    float vy = (orbitCenter.y + 2.5f) * std::sin(angle);
    glm::vec3 vPos = glm::vec3(vx, vy, vz);
    model = glm::translate(model, vPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(177.4f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, -angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)); // clockwise direction
    model = glm::scale(model, glm::vec3(0.712f)); 
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, venus);
    shader.setInt("venus", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
    
    // Mars 
    model = glm::mat4(1.0f); 
    angle = glfwGetTime() * 15.8f * timeScale; 
    orbitCenter = lightPos; // move to Earth postiion
    float mrx = (orbitCenter.x + ssize + 1.52f * sizeScale) * std::cos(angle);
    float mrz = (orbitCenter.z + ssize + 1.52f * sizeScale) * std::sin(angle);
    float mry = (orbitCenter.y + 2.5f) * std::sin(angle);
    glm::vec3 mrPos = glm::vec3(mrx, mry, mrz);
    model = glm::translate(model, mrPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(25.2f), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::rotate(model, angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.398f)); 
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mars);
    shader.setInt("mars", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

    // Jupiter 
    model = glm::mat4(1.0f); 
    angle = glfwGetTime() * 2.51f * timeScale; 
    orbitCenter = lightPos; // move to Earth postiion
    float jx = (orbitCenter.x + 5.20f * sizeScale) * std::cos(angle);
    float jz = (orbitCenter.z + 5.20f * sizeScale) * std::sin(angle);
    float jy = (orbitCenter.y + 2.5f) * std::sin(angle);
    glm::vec3 jPos = glm::vec3(jx, jy, jz);
    model = glm::translate(model, jPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(3.13f), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::rotate(model, angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(8.210f)); 
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, jupiter);
    shader.setInt("jupiter", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

    // Saturn 
    model = glm::mat4(1.0f); 
    angle = glfwGetTime() * 1.01f * timeScale; 
    orbitCenter = lightPos; // move to Earth postiion
    float sx = (orbitCenter.x + 9.58f * sizeScale) * std::cos(angle);
    float sz = (orbitCenter.z + 9.58f * sizeScale) * std::sin(angle);
    float sy = (orbitCenter.y + 2.5f) * std::sin(angle);
    glm::vec3 sPos = glm::vec3(sx, sy, sz);
    model = glm::translate(model, sPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(26.7f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(6.844f)); 
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, saturn);
    shader.setInt("saturn", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

    glm::mat4 ringModel = glm::mat4(1.0f); // Saturn ring 
    ringModel = glm::translate(ringModel, sPos);  // Position the rings at Saturn's location
    ringModel = glm::rotate(ringModel, glm::radians(26.7f), glm::vec3(0.0f, 0.0f, 1.0f)); // Align with Saturn's tilt
    ringModel = glm::rotate(ringModel, angle * 4.7f, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate rings around the Y axis (same as Saturn)
    ringModel = glm::scale(ringModel, glm::vec3(10.0f, 1.0f, 10.0f)); 
    shader.setMat4("model", ringModel); 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, saturnRing); 
    shader.setInt("saturnRing", 0);
    glBindVertexArray(VAO); // Ensure you have a separate VAO for the rings
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

    // Uranus 
    model = glm::mat4(1.0f); 
    angle = glfwGetTime() * 0.355f * timeScale; 
    orbitCenter = lightPos; // move to Earth postiion
    float ux = (orbitCenter.x + 19.18f * sizeScale) * std::cos(angle);
    float uz = (orbitCenter.z + 19.18f * sizeScale) * std::sin(angle);
    float uy = (orbitCenter.y + 2.5f) * std::sin(angle);
    glm::vec3 uPos = glm::vec3(ux, uy, uz);
    model = glm::translate(model, uPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(97.77f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, -angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.986f)); 
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, uranus);
    shader.setInt("uranus", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

    // Neptune scaled to 2.901f
    model = glm::scale(model, glm::vec3());
    model = glm::mat4(1.0f); 
    angle = glfwGetTime() * 0.181f * timeScale; 
    orbitCenter = lightPos; // move to Earth postiion
    float nx = (orbitCenter.x + 30.07f * sizeScale) * std::cos(angle);
    float nz = (orbitCenter.z + 30.07f * sizeScale) * std::sin(angle);
    float ny = (orbitCenter.y + 2.5f) * std::sin(angle);
    glm::vec3 nPos = glm::vec3(nx, ny, nz);
    model = glm::translate(model, nPos); // move at circumference of radius 
    model = glm::rotate(model, glm::radians(28.3f), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::rotate(model, angle * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.901f)); 
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, neptune);
    shader.setInt("neptune", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);

}
    
    void del() {  // cal this by object.del();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &lightVAO);
        glDeleteVertexArrays(1, &moonVAO);
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