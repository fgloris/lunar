#pragma once
#include "stb_image.h"
#include "window.hpp"

namespace lunar{

class Texture{
public:
    Texture(const std::string &filename,
    const unsigned int id,
    const unsigned int expand_param = GL_MIRRORED_REPEAT,
    const unsigned int filter_param_max = GL_LINEAR,
    const unsigned int filter_param_min = GL_NEAREST,
    const unsigned int texture_format = GL_RGB,
    const unsigned int source_img_format = GL_RGB,
    bool generate_mitmap = true
    );
    ~Texture();
    void use();
    unsigned int getID() const {return id;}
    std::string name;
private:
    unsigned int texture, id;
    int width, height, nrChannels;
};

}