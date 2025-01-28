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
        //void Draw(Shader &shader);
    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh();
};

namespace detail {
// 辅助类，用于加载模型时的临时数据
class ModelLoader {
public:
    explicit ModelLoader(const std::string& path);
    std::vector<Mesh> loadModel(const std::string& path);

private:
    std::string directory;
    std::vector<std::string> textures_loaded;
    
    std::vector<Mesh> processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type);
};
}

class Model {
    public:
        explicit Model(std::string path);
        //void Draw(Shader shader);   
    private:
        std::vector<Mesh> meshes;
};

}