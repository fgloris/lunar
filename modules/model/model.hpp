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

class Model {
    public:
        Model(std::string path)
        {
            loadModel(path);
        }
        //void Draw(Shader shader);   
    private:
        /*  模型数据  */
        /*  函数   */
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type);

        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<std::string> textures_loaded;
};
}