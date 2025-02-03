#include "postprocess.hpp"
#include "shader.hpp"
#include "window.hpp"
#include <stdexcept>
#include <iostream>

namespace lunar {

static const std::string postprocess_vertex_shader = 
#include "glsllibs/postprocess-vs.glsl"
;
static std::string postprocess_fragment_shader = ShaderProgram::loadGLSLlib(
    #include "glsllibs/postprocess-fs.glsl"
    ,
    #include "glsllibs/3shade2.glsl"
);

PostProcesser::PostProcesser():shader(postprocess_vertex_shader, postprocess_fragment_shader) {
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (!Window::initialized) {
        throw std::runtime_error("Window not initialized");
    }
    Window& w = Window::getInstance();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w.getWidth(), w.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w.getWidth(), w.getHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer is not complete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    shader.setVertices<4>({
    {-1.0f,  1.0f,  0.0f, 1.0f},
    {-1.0f, -1.0f,  0.0f, 0.0f},
    {1.0f, -1.0f,  1.0f, 0.0f},

    {-1.0f,  1.0f,  0.0f, 1.0f},
    {1.0f, -1.0f,  1.0f, 0.0f},
    {1.0f,  1.0f,  1.0f, 1.0f}
    });
    shader.setVertexDataProperty({"position", "TexCoords"}, {2, 2});
    shader.setIndices({0, 1, 2, 3, 4, 5});

}

void PostProcesser::tobeDrawn() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
}

void PostProcesser::toDraw() {
    shader.use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setInt("screenTexture", 0);
}

void PostProcesser::draw() {
    shader.draw();
}

}