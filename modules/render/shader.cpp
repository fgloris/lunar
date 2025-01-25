#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <glad/glad.h>

namespace lunar {
    Shader::Shader(int shader_type, const std::string& shader_code) : shader_type(shader_type) {
        const char* shader_code_cstr = shader_code.c_str();
        
        shader_id = glCreateShader(shader_type);
        glShaderSource(shader_id, 1, &shader_code_cstr, nullptr);
        glCompileShader(shader_id);

        int success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
            throw std::runtime_error("ERROR: SHADER COMPILATION FAILED\n" + std::string(infoLog));
        }
    }

    Shader::~Shader() {
        glDeleteShader(shader_id);
    }

    ShaderProgram::ShaderProgram(const std::string& vertex_shader_code, const std::string& fragment_shader_code, const std::string& compute_shader_code) {
        if (!vertex_shader_code.empty()) vertex_shader = new Shader(GL_VERTEX_SHADER, vertex_shader_code);
        if (!fragment_shader_code.empty()) fragment_shader = new Shader(GL_FRAGMENT_SHADER, fragment_shader_code);
        if (!compute_shader_code.empty()) compute_shader = new Shader(GL_COMPUTE_SHADER, compute_shader_code);
        attachShaders();

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);
    }

    ShaderProgram::~ShaderProgram() {
        delete vertex_shader;
        delete fragment_shader;
        delete compute_shader;
        glDeleteProgram(program_id);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void ShaderProgram::draw() const {
        glUseProgram(program_id);
        glDrawElements(GL_TRIANGLES, ebo_indices.size(), GL_UNSIGNED_INT, 0);
    }

    void ShaderProgram::use() const {
        glUseProgram(program_id);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    void ShaderProgram::attachShaders(){
        program_id = glCreateProgram();
        if (vertex_shader) glAttachShader(program_id, vertex_shader->getID());
        if (fragment_shader) glAttachShader(program_id, fragment_shader->getID());
        if (compute_shader) glAttachShader(program_id, compute_shader->getID());
        glLinkProgram(program_id);

        int success;
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program_id, 512, NULL, infoLog);
            throw std::runtime_error("ERROR: SHADER PROGRAM LINKING FAILED\n" + std::string(infoLog));
        }
    }
    
    void ShaderProgram::setIndices(std::vector<unsigned int> indices){
        ebo_indices = std::move(indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_indices.size() * sizeof(unsigned int), &ebo_indices[0], GL_STATIC_DRAW);
    }

    void ShaderProgram::setSequentialIndices(){
        ebo_indices = std::vector<unsigned int>(vertices.size() / 3);
        for (unsigned int i = 0; i < ebo_indices.size(); i++) {
            ebo_indices[i] = i;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_indices.size() * sizeof(unsigned int), &ebo_indices[0], GL_STATIC_DRAW);
    }

    void ShaderProgram::setVertexDataProperty(std::vector<std::string> names, std::vector<unsigned int> sizes){
        assert(names.size() == sizes.size());
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        unsigned int stride = 0, offset = 0;
        for (unsigned int i = 0; i < sizes.size(); i++) {
            stride += sizes[i];
        }
        
        for (unsigned int i = 0; i < names.size(); i++) {
            glVertexAttribPointer(i,                           
                             sizes[i],                         
                             GL_FLOAT,                         
                             GL_FALSE,                         
                             stride * sizeof(float),           
                             (void*)(offset * sizeof(float))); 
            glEnableVertexAttribArray(i);
            offset += sizes[i];
        }
    }
    
    void ShaderProgram::useTexture(const Texture& texture) const {
        texture.use();
        setInt(texture.name, texture.getID());
    }

    void ShaderProgram::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
    }

    void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void ShaderProgram::setVec3(const std::string &name, const glm::vec3 &vec) const {
        glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, glm::value_ptr(vec));
    }
    
    void ShaderProgram::unbindBuffers() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}