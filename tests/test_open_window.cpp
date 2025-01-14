#include "window.hpp"
#include "shader.hpp"
#include <gtest/gtest.h>
#include <memory>

class OpenGLTest : public ::testing::Test {
protected:
    void SetUp() override {
        window = &lunar::Window::getInstance();
        window->init(800, 600, "OpenGL Test");
    }

    void TearDown() override {
        // Window是单例，会自动清理
    }

    lunar::Window* window;
};

TEST_F(OpenGLTest, WindowInitialization) {
    EXPECT_NE(window->getHandle(), nullptr);
    EXPECT_EQ(window->getWidth(), 800);
    EXPECT_EQ(window->getHeight(), 600);
}

TEST_F(OpenGLTest, ShaderCompilation) {
    // 测试着色器编译
    EXPECT_NO_THROW({
        lunar::Shader vertex_shader(GL_VERTEX_SHADER, "modules/render/GLSL/vertex.glsl");
        lunar::Shader fragment_shader(GL_FRAGMENT_SHADER, "modules/render/GLSL/fragment.glsl");
    });
}

TEST_F(OpenGLTest, ShaderProgramLinking) {
    lunar::Shader vertex_shader(GL_VERTEX_SHADER, "modules/render/GLSL/vertex.glsl");
    lunar::Shader fragment_shader(GL_FRAGMENT_SHADER, "modules/render/GLSL/fragment.glsl");
    
    EXPECT_NO_THROW({
        lunar::ShaderProgram shader_program(vertex_shader, fragment_shader);
    });
}

TEST_F(OpenGLTest, BasicRendering) {
    lunar::Shader vertex_shader(GL_VERTEX_SHADER, "modules/render/GLSL/vertex.glsl");
    lunar::Shader fragment_shader(GL_FRAGMENT_SHADER, "modules/render/GLSL/fragment.glsl");
    lunar::ShaderProgram shader_program(vertex_shader, fragment_shader);

    // 测试一帧渲染
    EXPECT_NO_THROW({
        glClear(GL_COLOR_BUFFER_BIT);
        shader_program.draw();
        window->swapBuffers();
        window->pollEvents();
    });
}