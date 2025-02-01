#pragma once
#include "glm/glm.hpp"
#include "texture.hpp"
#include "material.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <map>
#include <vector>

namespace lunar {

class ShaderProgram;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, float shininess=32.0f);
    void Draw(ShaderProgram &shader);
private:
    float shininess;
    unsigned int VAO, VBO, EBO;
    void init();
};

class Model {
public:
    explicit Model(std::string path);
    void Draw(ShaderProgram &shader);   
private:
    std::vector<Mesh> meshes;
    glm::mat4 model;
    glm::mat3 normal_matrix;
};

namespace detail {
class ModelLoader {
public:
    explicit ModelLoader(const std::string& path);
    std::vector<Mesh> loadModel(const std::string& path);

private:
    std::string directory;
    std::vector<Texture> textures_loaded;
    const aiScene* scene;
    
    unsigned int processFBXFlags() const;
    bool isFBX(const std::string& path) const;
    
    std::vector<Mesh> processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(std::vector<Texture>& textures, aiMaterial *mat, aiTextureType type);
};
}

}