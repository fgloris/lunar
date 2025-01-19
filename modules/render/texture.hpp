#pragma once
#include "stb_image.h"
#include "window.hpp"

namespace lunar{

class Texture{
public:
    Texture(const std::string &filename,
    const unsigned int EXPAND_PARAM = GL_MIRRORED_REPEAT,
    const unsigned int FILTER_PARAM_MAX = GL_LINEAR,
    const unsigned int FILTER_PARAM_MIN = GL_NEAREST
    );
    ~Texture();
private:
    unsigned int texture;
    int width, height, nrChannels;
};

}