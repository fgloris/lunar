#include "shader.hpp"
#include "texture.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <glad.h>

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
        bindBuffers();
        unbindBuffers();
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(program_id);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    void ShaderProgram::draw() const {
        glUseProgram(program_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, ebo_indices.size(), GL_UNSIGNED_INT, 0);
    }

    void ShaderProgram::use() const {
        glUseProgram(program_id);
        // 设置纹理采样器的uniform
        setInt("texture1", 0);
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

    void ShaderProgram::setVertices(std::vector<float> vertices) {
        this->vertices = std::move(vertices);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);
    }

    void ShaderProgram::setIndicies(std::vector<unsigned int> indicies){
        this->ebo_indices = std::move(indicies);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->ebo_indices.size() * sizeof(unsigned int), &this->ebo_indices[0], GL_STATIC_DRAW);
    }

    void ShaderProgram::bindBuffers() {
        // VBO - 存储所有顶点数据
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // VAO - 配置如何解释数据
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // 配置位置属性
        glVertexAttribPointer(0,                // 位置属性的layout location
                             3,                 // 3个float (xyz)
                             GL_FLOAT,          // 数据类型
                             GL_FALSE,          // 不需要归一化
                             8 * sizeof(float), // 步长：8个float (xyz + rgb + uv)
                             (void*)0);         // 位置数据偏移量为0
        glEnableVertexAttribArray(0);

        // 配置颜色属性
        glVertexAttribPointer(1,                // 颜色属性的layout location
                             3,                 // 3个float (rgb)
                             GL_FLOAT,          // 数据类型
                             GL_FALSE,          // 不需要归一化
                             8 * sizeof(float), // 步长：8个float
                             (void*)(3 * sizeof(float))); // 颜色数据从第4个float开始
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2,
                             2, 
                             GL_FLOAT, 
                             GL_FALSE, 
                             8 * sizeof(float), 
                             (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    void ShaderProgram::unbindBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void ShaderProgram::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
    }
}