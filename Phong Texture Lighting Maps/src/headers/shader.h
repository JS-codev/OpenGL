
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//`Shader anything` to call constructor of Shader() class
//TL;TR: Creates a new shader object called "anything" in main.cpp from shader() constructor
class Shader {
public:
    unsigned int ID;

    // Constructor using file paths
    Shader(const char* vertexPath, const char* fragmentPath) {
        // empty boxes to store stuff (variables)
        std::string vertexCode; 
        std::string fragmentCode;

        // reading glasses (ifstream object can read .txt file inside openGL)
        std::ifstream vShaderFile;  
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        // e.g "If the file is broken or missing, tell me".
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try // try to read the files, like opening a book.
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            // Takes all the words from the book (shader file) and help turn them into strings.
            std::stringstream vShaderStream, fShaderStream;

            // Read & copy Vertex/Frag file's buffer contents, and then put into streams (vShaderStream, fShaderStream)
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();	

            // close files
            vShaderFile.close();
            fShaderFile.close();

            // convert stream into string, then put them into empty boxes
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
        }

        // If cannot read file, produce error. 
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        
        // Turns them into C-style strings, because OpenGL needs it that way.
        const char* VertexShader = vertexCode.c_str();
        const char * FragmentShader = fragmentCode.c_str();
     
        // Compile vertex shader
        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &VertexShader, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // Compile fragment shader
        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &FragmentShader, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // Link shaders into program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader( ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // Cleanup
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    //`shader.use()` (in main.cpp) to activate this shader render program
    void use() const { // const to make it "read-only"
        glUseProgram(ID); 
    }

    // Ingeter number
    void setInt(const std::string &name, int value) const { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    
    // float number - 128.0f 
    void setFloat(const std::string &name, float value) const { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }

    void setVec3(const std::string &name, float x, float y, float z) const { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
  void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

//Error checking...
private:
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- ------ " 
                          << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- ----" 
                          << std::endl;
            }
        }
    }
};

#endif