#pragma once
#include "glm/glm.hpp"
#include "texture.hpp"
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
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(ShaderProgram &shader, unsigned int dif, unsigned int spec);
private:
    unsigned int VAO, VBO, EBO;
    void init();
};

class Model {
public:
    explicit Model(std::string path);
    void Draw(ShaderProgram &shader, Texture& dif, Texture& spec);   
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
    
    std::vector<Mesh> processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(std::vector<Texture>& textures, aiMaterial *mat, aiTextureType type);
};
}

}