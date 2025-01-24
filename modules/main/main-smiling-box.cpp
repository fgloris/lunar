#include "render/window.hpp"
#include "render/shader.hpp"
#include "render/camera.hpp"
#include "render/texture.hpp"
#include "interface/interface.hpp"
#include <iostream>
#include <functional>

int main() {
    auto& window = lunar::Window::getInstance();
    try {
        window.init(1800, 1200, "OpenGL");
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize window, error: " << e.what() << std::endl;
        return -1;
    }
    const std::string vertex_shader_code = 
    #include "../modules/render/GLSL/vertex.glsl"
    ;
    const std::string fragment_shader_code = 
    #include "../modules/render/GLSL/fragment.glsl"
    ;
    lunar::ShaderProgram shader_program(vertex_shader_code, fragment_shader_code);
    shader_program.addVertexDataProperty({"position","color","texture"},{3,3,2});
    shader_program.bindVAO(0);
    shader_program.setVertices<8>({
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f},

        {-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        {-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f},

        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        {-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        {-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

        {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
        {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f}
    });
    shader_program.setSequentialIndices();

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

    lunar::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    lunar::Interface& interface = lunar::Interface::getInstance();

    interface.registerCallback("window_close", std::bind(&lunar::Window::close, &window, std::placeholders::_1));
    interface.registerCallback("window_fullscreen", std::bind(&lunar::Window::fullscreen, &window, std::placeholders::_1));
    interface.registerCallback("window_windowed", std::bind(&lunar::Window::windowed, &window, std::placeholders::_1));
    interface.registerCallback("camera_move_forward", std::bind(&lunar::Camera::MoveForward, &camera, std::placeholders::_1));
    interface.registerCallback("camera_move_backward", std::bind(&lunar::Camera::MoveBackward, &camera, std::placeholders::_1));
    interface.registerCallback("camera_move_left", std::bind(&lunar::Camera::MoveLeft, &camera, std::placeholders::_1));
    interface.registerCallback("camera_move_right", std::bind(&lunar::Camera::MoveRight, &camera, std::placeholders::_1));
    interface.registerCallback("camera_rotate", std::bind(&lunar::Camera::Rotate, &camera, std::placeholders::_1));
    interface.registerCallback("camera_reset_zoom", std::bind(&lunar::Camera::resetZoom, &camera));

    interface.registerCallback("camera_zoom", std::bind(&lunar::Camera::Zoom, &camera, std::placeholders::_1));
    interface.bindAllCallbacks("../modules/config/interface.yaml", window.getHandle());

    glEnable(GL_DEPTH_TEST);
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 1.0f, 0.0f));

        shader_program.use();
        shader_program.setTransform(camera.computeProjectionMatrix() * camera.computeViewMatrix() * model);
        shader_program.draw();
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}