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
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    vec = trans * vec;
    std::cout << vec.x << ' ' << vec.y << ' '  << vec.z << std::endl;

    lunar::ShaderProgram shader_program;
    shader_program.setVertices({
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 左下，红色
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // 右下，绿色
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // 顶部，蓝色
    });
    shader_program.use();
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        shader_program.draw();
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}