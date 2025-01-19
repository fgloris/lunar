#include "texture.hpp"
#include <fstream>
#include <iostream>

namespace lunar{

Texture::Texture(const std::string &filename,
    const unsigned int EXPAND_PARAM,
    const unsigned int FILTER_PARAM_MAX,
    const unsigned int FILTER_PARAM_MIN
    ){
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, EXPAND_PARAM);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, EXPAND_PARAM);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FILTER_PARAM_MAX);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FILTER_PARAM_MIN);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else std::cerr << "Failed to load texture" << std::endl;
    stbi_image_free(data);
}

Texture::~Texture(){
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
}

}