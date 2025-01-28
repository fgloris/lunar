#include "render/window.hpp"
#include "render/shader.hpp"
#include "render/camera.hpp"
#include "render/texture.hpp"
#include "interface/interface.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <functional>
class SmilingBoxTest : public ::testing::Test {
protected:
    void SetUp() override {
        window = &lunar::Window::getInstance();
        window->init(1800, 1200, "OpenGL Test");
        
        const std::string vertex_shader_code = 
        #include "smiling-box/smiling-box-vs.glsl"
        ;
        const std::string fragment_shader_code = 
        #include "smiling-box/smiling-box-fs.glsl"
        ;
        
        shader_program = std::make_unique<lunar::ShaderProgram>(vertex_shader_code, fragment_shader_code);
        camera = std::make_unique<lunar::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        setupShaderAndGeometry();
        setupTextures();
        
        // 只在第一次测试时注册回调
        static bool callbacks_registered = false;
        if (!callbacks_registered) {
            setupCallbacks();
            callbacks_registered = true;
        }
    }

    void TearDown() override {
        shader_program.reset();
        camera.reset();
    }

    void setupShaderAndGeometry() {
        shader_program->setVertexDataProperty({"position","color","texture"},{3,3,2});
        shader_program->setVertices<8>({
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
        shader_program->setSequentialIndices();
    }

    void setupTextures() {
        texture1 = std::make_unique<lunar::Texture>("../assets/container.jpg", 0);
        texture2 = std::make_unique<lunar::Texture>("../assets/awesomeface.png", 1);
        
        shader_program->use();
        shader_program->useTexture(*texture1);
        shader_program->useTexture(*texture2);
    }

    void setupCallbacks() {
        auto& interface = lunar::Interface::getInstance();
        
        // 在注册前先清除所有已存在的回调
        interface.clearCallbacks();  // 需要在Interface类中添加这个方法
        
        interface.registerCallback("window_close", std::bind(&lunar::Window::close, window, std::placeholders::_1));
        interface.registerCallback("window_fullscreen", std::bind(&lunar::Window::fullscreen, window, std::placeholders::_1));
        interface.registerCallback("window_windowed", std::bind(&lunar::Window::windowed, window, std::placeholders::_1));
        interface.registerCallback("camera_move_forward", std::bind(&lunar::Camera::moveForward, camera.get(), std::placeholders::_1));
        interface.registerCallback("camera_move_backward", std::bind(&lunar::Camera::moveBackward, camera.get(), std::placeholders::_1));
        interface.registerCallback("camera_move_left", std::bind(&lunar::Camera::moveLeft, camera.get(), std::placeholders::_1));
        interface.registerCallback("camera_move_right", std::bind(&lunar::Camera::moveRight, camera.get(), std::placeholders::_1));
        interface.registerCallback("camera_rotate", std::bind(&lunar::Camera::rotate, camera.get(), std::placeholders::_1));
        interface.registerCallback("camera_reset_zoom", std::bind(&lunar::Camera::resetZoom, camera.get()));
        interface.registerCallback("camera_zoom", std::bind(&lunar::Camera::zoom, camera.get(), std::placeholders::_1));
        interface.registerCallback("camera_move_up", std::bind(&lunar::Camera::moveUp, camera.get(), std::placeholders::_1));
        interface.registerCallback("camera_move_down", std::bind(&lunar::Camera::moveDown, camera.get(), std::placeholders::_1));
        interface.bindAllCallbacks("../modules/config/interface.yaml", window->getHandle());
    }

    lunar::Window* window;
    std::unique_ptr<lunar::ShaderProgram> shader_program;
    std::unique_ptr<lunar::Camera> camera;
    std::unique_ptr<lunar::Texture> texture1;
    std::unique_ptr<lunar::Texture> texture2;
};

TEST_F(SmilingBoxTest, InitializationTest) {
    EXPECT_NE(window->getHandle(), nullptr);
    EXPECT_NE(shader_program.get(), nullptr);
    EXPECT_NE(camera.get(), nullptr);
    EXPECT_NE(texture1.get(), nullptr);
    EXPECT_NE(texture2.get(), nullptr);
}

TEST_F(SmilingBoxTest, RenderOneFrame) {
    EXPECT_NO_THROW({
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 1.0f, 0.0f));

        shader_program->use();
        shader_program->setMat4("transform", camera->computeProjectionMatrix() * camera->computeViewMatrix() * model);
        shader_program->draw();
        window->swapBuffers();
        window->pollEvents();
    });
}

TEST_F(SmilingBoxTest, TextureBinding) {
    EXPECT_NO_THROW({
        shader_program->use();
        shader_program->useTexture(*texture1);
        shader_program->useTexture(*texture2);
    });
}