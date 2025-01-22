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
    
    Shader::Shader(int shader_type) : shader_type(shader_type) {
        std::string shader_code;
        switch (shader_type) {
        case GL_VERTEX_SHADER:
            shader_code = 
            #include "GLSL/vertex.glsl"
            ;
            break;
        case GL_FRAGMENT_SHADER:
            shader_code = 
            #include "GLSL/fragment.glsl"
            ;
            break;
        case GL_COMPUTE_SHADER:
            shader_code = 
            #include "GLSL/compute.glsl"
            ;
            break;
        default:
            throw std::runtime_error("ERROR: SHADER TYPE NOT SUPPORTED\n");
        }
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

    ShaderProgram::ShaderProgram(Shader& vertex_shader, Shader& fragment_shader) {
        attachShaders(vertex_shader, fragment_shader);
        bindBuffers();
        unbindBuffers();
    }

    ShaderProgram::ShaderProgram() {
        Shader vertex_shader(GL_VERTEX_SHADER);
        Shader fragment_shader(GL_FRAGMENT_SHADER);
        attachShaders(vertex_shader, fragment_shader);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);

        bindBuffers();
    }

    ShaderProgram::~ShaderProgram() {
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
        bindBuffers();
    }

    void ShaderProgram::attachShaders(Shader& vertex_shader, Shader& fragment_shader){
        program_id = glCreateProgram();
        glAttachShader(program_id, vertex_shader.getID());
        glAttachShader(program_id, fragment_shader.getID());
        glLinkProgram(program_id);

        int success;
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program_id, 512, NULL, infoLog);
            throw std::runtime_error("ERROR: SHADER PROGRAM LINKING FAILED\n" + std::string(infoLog));
        }
    }
    
    void ShaderProgram::setIndicies(std::vector<unsigned int> indicies){
        ebo_indices = std::move(indicies);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_indices.size() * sizeof(unsigned int), &ebo_indices[0], GL_STATIC_DRAW);
    }

    void ShaderProgram::setSequentialIndicies(){
        ebo_indices = std::vector<unsigned int>(vertices.size() / 5);
        for (unsigned int i = 0; i < ebo_indices.size(); i++) {
            ebo_indices[i] = i;
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_indices.size() * sizeof(unsigned int), &ebo_indices[0], GL_STATIC_DRAW);
    }

    void ShaderProgram::setVertexDataProperty(std::vector<std::string> names, std::vector<unsigned int> sizes){
        assert(names.size() == sizes.size());
        glBindVertexArray(VAO);
        unsigned int stride = 0, offset = 0;
        for (unsigned int i = 0; i < sizes.size(); i++) {
            stride += sizes[i];
        }
        for (unsigned int i = 0; i < names.size(); i++) {
            glVertexAttribPointer(i,                                // 位置属性的layout location
                             sizes[i],                              // 3个float (xyz)
                             GL_FLOAT,                              // 数据类型
                             GL_FALSE,                              // 不需要归一化
                             stride * sizeof(float),                // 步长：8个float (xyz + rgb + uv)
                             (void*)(offset * sizeof(float)));      // 位置数据偏移量为0
            glEnableVertexAttribArray(i);
            offset += sizes[i];
        }
    }

    void ShaderProgram::bindBuffers() const {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    void ShaderProgram::unbindBuffers() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void ShaderProgram::useTexture(const Texture& texture) const {
        texture.use();
        setInt(texture.name, texture.getID());
    }

    void ShaderProgram::setTransform(const glm::mat4 &mat) const {
        setMat4("transform", mat);
    }

    void ShaderProgram::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
    }

    void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
}