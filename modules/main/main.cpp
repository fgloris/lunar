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

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 
                                static_cast<float>(window.getWidth()) / static_cast<float>(window.getHeight()),
                                0.1f, 
                                100.0f);
    glm::mat4 transform;
    while (!window.shouldClose()) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = projection * view * model;

        glClear(GL_COLOR_BUFFER_BIT);
        shader_program.use();
        shader_program.setTransform(transform);
        shader_program.draw();
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}