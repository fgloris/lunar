#include "shader.hpp"
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

    void ShaderProgram::setVertices(const std::vector<float>& vertices) {
        this->vertices = vertices;
    }

    void ShaderProgram::bindBuffers() {
        // 顶点数据：位置(xyz) + 颜色(rgb)
        vertices = {
            // 位置          // 颜色
            -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 左下，红色
             0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // 右下，绿色
             0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // 顶部，蓝色
        };

        // VBO - 存储所有顶点数据
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        // VAO - 配置如何解释数据
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // 配置位置属性
        glVertexAttribPointer(0,                // 位置属性的layout location
                             3,                 // 3个float (xyz)
                             GL_FLOAT,          // 数据类型
                             GL_FALSE,          // 不需要归一化
                             6 * sizeof(float), // 步长：6个float (xyz + rgb)
                             (void*)0);         // 位置数据偏移量为0
        glEnableVertexAttribArray(0);

        // 配置颜色属性
        glVertexAttribPointer(1,                // 颜色属性的layout location
                             3,                 // 3个float (rgb)
                             GL_FLOAT,          // 数据类型
                             GL_FALSE,          // 不需要归一化
                             6 * sizeof(float), // 步长：6个float
                             (void*)(3 * sizeof(float))); // 颜色数据从第4个float开始
        glEnableVertexAttribArray(1);
    }

    void ShaderProgram::unbindBuffers() {
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0);
    }

    Shader getShader(int shader_type){
        switch (shader_type)
        {
        case GL_VERTEX_SHADER:{
            const std::string source =
            #include "GLSL/vertex.glsl"
            ;
            return Shader(shader_type, source);
        }
        case GL_FRAGMENT_SHADER:{
            const std::string source =
            #include "GLSL/fragment.glsl"
            ;
            return Shader(shader_type, source);
        }
        
        default:
            throw std::runtime_error("ERROR: SHADER TYPE NOT SUPPORTED\n");
        }
    }
}