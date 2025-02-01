#pragma once
#include "shader.hpp"
namespace lunar {
class PostProcesser {
public:
    PostProcesser();
    void tobeDrawn();
    void toDraw();
    void draw();
private:
    ShaderProgram shader;
    unsigned int framebuffer, texture, renderbuffer;
};

}



