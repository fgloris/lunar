#include "texture.hpp"
#include <fstream>
#include <iostream>

namespace lunar{

Texture::Texture(const std::string &filename,
    TextureType type,
    const unsigned int expand_param,
    const unsigned int filter_param_max,
    const unsigned int filter_param_min,
    bool generate_mitmap,
    bool flip_y
    ):type(type){
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    stbi_set_flip_vertically_on_load(flip_y);
    
    // 加载图片并获取通道数
    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    
    // 根据通道数选择合适的格式
    GLenum format;
    switch(channels) {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: format = GL_RGB; break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, expand_param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, expand_param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_param_max);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_param_min);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        if (generate_mitmap){
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    else std::cerr << "Failed to load texture: " << filename << std::endl;
    stbi_image_free(data);
}

Texture::~Texture(){
    glDeleteTextures(1, &id);
}

}