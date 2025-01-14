#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <glad.h>

namespace lunar {
    Shader::Shader(int shader_type, const std::string& shader_path) : shader_type(shader_type) {
        std::ifstream file(shader_path);
        if (!file.is_open()) {
            throw std::runtime_error("ERROR: SHADER FILE NOT FOUND: " + shader_path);
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string shader_code = buffer.str();
        const char* shader_code_cstr = shader_code.c_str();
        
        shader_id = glCreateShader(shader_type);
        glShaderSource(shader_id, 1, &shader_code_cstr, nullptr);
        glCompileShader(shader_id);

        int success;
        char infoLog[512];
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
            throw std::runtime_error("ERROR: SHADER COMPILATION FAILED\n" + std::string(infoLog));
        }
    }
    
    Shader::~Shader() {
        glDeleteShader(shader_id);
    }

    ShaderProgram::ShaderProgram(Shader& vertex_shader, Shader& fragment_shader) {
        attachShaders(vertex_shader, fragment_shader);

        vertices = {
            -0.5f, -0.5f, 0.0f, // left  
             0.5f, -0.5f, 0.0f, // right 
             0.0f,  0.5f, 0.0f  // top   
        };

        bindBuffers();
        unbindBuffers();
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(program_id);
    }

    void ShaderProgram::draw() const {
        glUseProgram(program_id);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void ShaderProgram::attachShaders(Shader& vertex_shader, Shader& fragment_shader){
        program_id = glCreateProgram();
        glAttachShader(program_id, vertex_shader.getID());
        glAttachShader(program_id, fragment_shader.getID());
        glLinkProgram(program_id);

        int success;
        char infoLog[512];
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program_id, 512, NULL, infoLog);
            throw std::runtime_error("ERROR: SHADER PROGRAM LINKING FAILED\n" + std::string(infoLog));
        }
    }

    void ShaderProgram::bindBuffers(){
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ebo_indices), &ebo_indices[0], GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void ShaderProgram::unbindBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0);
    }
}