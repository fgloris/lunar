#include "window.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include <iostream>

int main() {
    auto& window = lunar::Window::getInstance();
    try {
        window.init(1800, 1200, "OpenGL");
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize window, error: " << e.what() << std::endl;
        return -1;
    }
    lunar::ShaderProgram shader_program;
    shader_program.setVertices({
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    });
    shader_program.setIndicies({
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    });

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    lunar::Texture texture1("../assets/container.jpg", 0);
    lunar::Texture texture2("../assets/awesomeface.png", 1,
                            GL_MIRRORED_REPEAT,
                            GL_LINEAR,
                            GL_NEAREST,
                            GL_RGBA,
                            GL_RGBA,
                            false, 
                            true);
    
    shader_program.use();
    shader_program.useTexture(texture1);
    shader_program.useTexture(texture2);
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        shader_program.use();
        shader_program.draw();
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}