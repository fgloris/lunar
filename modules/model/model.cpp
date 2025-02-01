#include "model.hpp"
#include "render/shader.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>

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
    // 绘制网格
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

bool ModelLoader::isFBX(const std::string& path) const {
    std::string extension = path.substr(path.find_last_of('.') + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return extension == "fbx";
}

unsigned int ModelLoader::processFBXFlags() const {
    // FBX特定的处理标志
    return aiProcess_Triangulate |              // 将所有图元转换为三角形
           aiProcess_FlipUVs |                  // 翻转纹理的Y坐标
           aiProcess_GenNormals |               // 如果模型没有法线则创建法线
           aiProcess_CalcTangentSpace |         // 计算切线和副切线
           aiProcess_JoinIdenticalVertices |    // 合并相同的顶点
           aiProcess_GlobalScale |              // 统一缩放
           aiProcess_LimitBoneWeights |         // 限制骨骼权重
           aiProcess_PopulateArmatureData |     // 填充骨骼数据
           aiProcess_SortByPType |              // 按图元类型排序
           aiProcess_FindDegenerates |          // 查找并删除退化的三角形
           aiProcess_FindInvalidData |          // 查找无效数据
           aiProcess_OptimizeMeshes;            // 优化网格
}

std::vector<Mesh> ModelLoader::loadModel(const std::string& path) {
    Assimp::Importer import;
    unsigned int flags = isFBX(path) ? processFBXFlags() : 
        (aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    
    scene = import.ReadFile(path, flags);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(std::string("Model Import Error: ASSIMP::") + import.GetErrorString());
    }

    // 检查是否有网格
    if(scene->mNumMeshes == 0) {
        throw std::runtime_error("Model has no meshes");
    }

    // 检查材质
    if(scene->mNumMaterials == 0) {
        std::cout << "Warning: Model has no materials" << std::endl;
    }

    try {
        return processNode(scene->mRootNode, scene);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to process model: ") + e.what());
    }
}

std::vector<Mesh> ModelLoader::processNode(aiNode *node, const aiScene *scene) {
    std::vector<Mesh> meshes;
    
    // 处理当前节点的所有网格
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        try {
            meshes.push_back(processMesh(mesh, scene));
        } catch (const std::exception& e) {
            std::cerr << "Warning: Failed to process mesh " << i << ": " << e.what() << std::endl;
            continue;
        }
    }
    
    // 递归处理子节点
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
    TextureType texture_type = TextureType::Diffuse;
    if (type == aiTextureType_DIFFUSE) {
        texture_type = TextureType::Diffuse;
    } else if (type == aiTextureType_SPECULAR) {
        texture_type = TextureType::Specular;
    }

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        
        // 首先尝试从嵌入纹理中加载
        const aiTexture* embeddedTex = scene->GetEmbeddedTexture(str.C_Str());
        if(embeddedTex != nullptr) {
            // 处理嵌入纹理
            int width, height, channels;
            unsigned char* data;
            
            if(embeddedTex->mHeight == 0) {
                // 压缩纹理
                data = stbi_load_from_memory(
                    reinterpret_cast<unsigned char*>(embeddedTex->pcData),
                    embeddedTex->mWidth,  // mWidth在这种情况下包含完整的数据大小
                    &width, &height, &channels, 0
                );
            } else {
                // 未压缩纹理
                width = embeddedTex->mWidth;
                height = embeddedTex->mHeight;
                channels = 4; // 假设RGBA
                data = reinterpret_cast<unsigned char*>(embeddedTex->pcData);
            }

            if(data) {
                // 使用现有的Texture构造函数创建纹理
                std::string embedded_name = std::string("embedded_") + str.C_Str();
                Texture texture(
                    embedded_name,           // 文件名（用于标识）
                    texture_type,           // 纹理类型
                    GL_REPEAT,             // 展开参数
                    GL_LINEAR,             // 放大过滤
                    GL_LINEAR_MIPMAP_LINEAR, // 缩小过滤
                    true,                  // 生成mipmap
                    false,                 // 不翻转Y轴
                    data,                  // 图像数据
                    width,                 // 宽度
                    height,                // 高度
                    channels              // 通道数
                );

                textures.push_back(texture);
                textures_loaded.push_back(texture);

                // 如果是压缩纹理，需要释放数据
                if(embeddedTex->mHeight == 0) {
                    stbi_image_free(data);
                }
                continue;
            }
        }

        // 如果没有嵌入纹理，尝试从文件加载
        std::string path = directory + '/' + str.C_Str();
        auto it = std::find(textures_loaded.begin(), textures_loaded.end(), path);
        if (it == textures_loaded.end()) {
            try {
                Texture texture(path, texture_type);
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            } catch (const std::exception& e) {
                std::cerr << "Warning: Failed to load texture: " << path << std::endl;
            }
        } else {
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