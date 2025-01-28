#include "model.hpp"
#include "render/shader.hpp"
#include "material.hpp"
#include <stdexcept>
#include <iostream>

namespace lunar {

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, float shininess):
    vertices(vertices), indices(indices), textures(textures), shininess(shininess) {
    init();
}

void Mesh::Draw(ShaderProgram &shader) {
    for(unsigned int i = 0; i < textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name;
        TextureType type = textures[i].type;
        if(type == TextureType::Diffuse)
            name = std::string("material.diffuse");
        else if(type == TextureType::Specular)
            name = std::string("material.specular");
        shader.setInt(name.c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    shader.setFloat("material.shininess", shininess);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::init(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glBindVertexArray(0);
}

namespace detail {

ModelLoader::ModelLoader(const std::string& path) {
    directory = path.substr(0, path.find_last_of('/'));
}

std::vector<Mesh> ModelLoader::loadModel(const std::string& path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);    

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(std::string("Model Import Error: ASSIMP::") + import.GetErrorString());
    }
    
    return processNode(scene->mRootNode, scene);
}

std::vector<Mesh> ModelLoader::processNode(aiNode *node, const aiScene *scene) {
    std::vector<Mesh> meshes;
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));         
    }
    
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        auto childMeshes = processNode(node->mChildren[i], scene);
        meshes.insert(meshes.end(), childMeshes.begin(), childMeshes.end());
    }
    return meshes;
}

Mesh ModelLoader::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        if(mesh->mTextureCoords[0]) {
            vertex.tex_coords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }
        else vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        loadMaterialTextures(textures, material, aiTextureType_DIFFUSE);
        loadMaterialTextures(textures, material, aiTextureType_SPECULAR);
    }

    return Mesh(vertices, indices, textures);
}

void ModelLoader::loadMaterialTextures(std::vector<Texture>& textures, aiMaterial *mat, aiTextureType type) {
    TextureType texture_type;
    if (type == aiTextureType_DIFFUSE) {
        texture_type = TextureType::Diffuse;
    }else if (type == aiTextureType_SPECULAR) {
        texture_type = TextureType::Specular;
    }
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = directory + '/' + str.C_Str();
        auto it = std::find(textures_loaded.begin(), textures_loaded.end(), path);
        if (it == textures_loaded.end()) {
            Texture texture(path, texture_type);
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }else{
            textures.push_back(*it);
        }
    }
}

} // namespace detail

Model::Model(std::string path) {
    detail::ModelLoader loader(path);
    meshes = loader.loadModel(path);
    model = glm::mat4(1.0f);
    normal_matrix = lunar::General::getNormalMatrix(model);
}

void Model::Draw(ShaderProgram &shader) {
    shader.setMat3("normalMatrix", normal_matrix);
    shader.setMat4("model", model);
    for(auto &mesh : meshes) {
        mesh.Draw(shader);
    }
}

} // namespace lunar