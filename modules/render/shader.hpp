#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "texture.hpp"

namespace lunar {
    class Shader {
    public:
        Shader(int shader_type, const std::string& shader_path);
        Shader(int shader_type);
        ~Shader();
        unsigned int getID() const { return shader_id; }
        friend class ShaderProgram;
    private:
        unsigned int shader_type;
        unsigned int shader_id;
    };

    class ShaderProgram {
    public:
        ShaderProgram(Shader& vertex_shader, Shader& fragment_shader);
        ShaderProgram();
        ~ShaderProgram();
        void use() const;
        void draw() const;
        void setIndicies(std::vector<unsigned int> indicies);
        void setVertices(std::vector<float> vertices);
        void useTexture(const Texture& texture) const;
        void setTransform(const glm::mat4 &mat) const;
        unsigned int getID() const {return program_id;}
    private:
        void setInt(const std::string &name, int value) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        void attachShaders(Shader& vertex_shader, Shader& fragment_shader);
        void bindBuffers();
        void unbindBuffers();

        std::vector<float> vertices;
        std::vector<unsigned int> ebo_indices;
        unsigned int VBO, VAO, EBO; // Vertex Buffer Object, Vertex Array Object, Element Buffer Object
        unsigned int program_id;
    };
}