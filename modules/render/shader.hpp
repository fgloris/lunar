#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "texture.hpp"
#include <type_traits>

namespace lunar {
    template<unsigned int N>
    struct VertexData{
        float data[N];
    };

    struct Material {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };

    class Shader {
    public:
        Shader(int shader_type, const std::string& shader_code);
        ~Shader();
        [[nodiscard]] unsigned int getID() const { return shader_id; }
        friend class ShaderProgram;
    private:
        unsigned int shader_type;
        unsigned int shader_id;
    };

    class ShaderProgram {
    public:
        ShaderProgram(const std::string& vertex_shader_code = "", const std::string& fragment_shader_code = "", const std::string& compute_shader_code = "");
        ~ShaderProgram();
        void use() const;
        void draw() const;
        void setIndices(std::vector<unsigned int> indices);
        void setSequentialIndices();
        void setSequentialIndices(unsigned int size);
        template<unsigned int N>
        void setVertices(std::vector<VertexData<N>> vertices){
            const float* raw_data = reinterpret_cast<const float*>(vertices.data());
            this->vertices = std::vector<float>(raw_data, raw_data + vertices.size() * N);
            
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &this->vertices[0], GL_STATIC_DRAW);
            stride = N;
        }
        void setVertexDataProperty(std::vector<std::string> names, std::vector<unsigned int> sizes);
        void useTexture(const Texture& texture) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, const glm::vec2 &vec) const;
        void setVec3(const std::string &name, const glm::vec3 &vec) const;
        void setVec4(const std::string &name, const glm::vec4 &vec) const;
        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        [[nodiscard]] unsigned int getID() const {return program_id;}

        std::vector<float> vertices;
        std::vector<unsigned int> ebo_indices;

        // 通用结构体设置模板
        template<typename T>
        void setUniformStruct(const std::string& name, const T& data) {
            if constexpr (std::is_same_v<T, Material>) {
                setVec3(name + ".ambient", data.ambient);
                setVec3(name + ".diffuse", data.diffuse);
                setVec3(name + ".specular", data.specular);
                setFloat(name + ".shininess", data.shininess);
            }else {
                static_assert(always_false<T>::value, "Unsupported uniform struct type");
            }
        }

    private:
        void attachShaders();
        void unbindBuffers() const;

        Shader* vertex_shader = nullptr, *fragment_shader = nullptr, *compute_shader = nullptr;
        unsigned int vertex_data_size;
        unsigned int VBO, EBO, VAO; // Vertex Buffer Object, Vertex Array Object, Element Buffer Object
        unsigned int program_id;
        unsigned int stride{0};

        template<typename T>
        struct always_false : std::false_type {};
    };
}