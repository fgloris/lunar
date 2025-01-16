#pragma once
#include <string>
#include <vector>

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
        void setVertices(std::vector<float> vertices);
    private:
        void attachShaders(Shader& vertex_shader, Shader& fragment_shader);
        void bindBuffers();
        void unbindBuffers();

        std::vector<float> vertices;
        std::vector<unsigned int> ebo_indices;
        unsigned int VBO, VAO, EBO; // Vertex Buffer Object, Vertex Array Object, Element Buffer Object
        unsigned int program_id;
    };
}