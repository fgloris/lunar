#pragma once
#include <string>
#include <vector>

namespace lunar {
    class Shader {
    public:
        Shader(int shader_type, const std::string& shader_path);
        ~Shader();
        unsigned int getID() const { return shader_id; }
    private:
        unsigned int shader_type;
        unsigned int shader_id;
    };

    class ShaderProgram {
    public:
        ShaderProgram(Shader& vertex_shader, Shader& fragment_shader);
        ~ShaderProgram();
        void draw() const;
    private:
        std::vector<float> vertices;
        unsigned int VBO, VAO; // Vertex Buffer Object, Vertex Array Object
        unsigned int program_id;
    };
}