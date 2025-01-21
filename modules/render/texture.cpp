#include "texture.hpp"
#include <fstream>
#include <iostream>

namespace lunar{

Texture::Texture(const std::string &filename,
    const unsigned int id,
    const unsigned int expand_param,
    const unsigned int filter_param_max,
    const unsigned int filter_param_min,
    const unsigned int texture_format,
    const unsigned int source_img_format,
    bool generate_mitmap,
    bool flip_y
    ):id(id){
    name = std::string("texture")+std::to_string(id);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    stbi_set_flip_vertically_on_load(flip_y);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nr_channels, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, expand_param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, expand_param);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_param_max);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_param_min);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, texture_format, width, height, 0, source_img_format, GL_UNSIGNED_BYTE, data);
        if (generate_mitmap){
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else std::cerr << "Failed to load texture" << std::endl;
    stbi_image_free(data);
}

void Texture::use() const {
    glActiveTexture(GL_TEXTURE0 + id);  // 始终使用纹理单元0
    glBindTexture(GL_TEXTURE_2D, texture);
}

Texture::~Texture(){
    glDeleteTextures(1, &texture);
}

}