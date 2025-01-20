#pragma once
#include "stb_image.h"
#include "window.hpp"

namespace lunar{

class Texture{
public:
    Texture(const std::string &filename,
    const unsigned int expand_param = GL_MIRRORED_REPEAT,
    const unsigned int filter_param_max = GL_LINEAR,
    const unsigned int filter_param_min = GL_NEAREST,
    const unsigned int texture_format = GL_RGB,
    const unsigned int source_img_format = GL_RGB,
    bool generate_mitmap = true
    );
    ~Texture();
    void use();
    unsigned int getID() {return texture;}
private:
    unsigned int texture;
    int width, height, nrChannels;
};

}