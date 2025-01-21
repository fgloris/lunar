#include "window.hpp"
#include <stdexcept>

namespace lunar {
    Window::~Window() {
        if (window) {
            glfwDestroyWindow(window);
        }
        if (initialized) {
            glfwTerminate();
        }
    }

    void Window::init(int width, int height, const std::string& title, bool isFullscreen){
        this->width = width;
        this->height = height;
        this->title = title;
        this->isFullscreen = isFullscreen;
        initGLFW();
        initWindow();
        initGLAD();
        initialized = true;
    }

    void Window::initGLFW() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void Window::initWindow() {
        if (isFullscreen) {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);
        } else {
            window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        }
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCursorPosCallback(window, mouseCallback);
    }

    void Window::initGLAD() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    void Window::processInput() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (initialized) {
            Window& instance = getInstance();
            for (const auto& callback : instance.key_callbacks) {
                callback(key, action);
            }
        }
    }

    void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        if (initialized) {
            Window& instance = getInstance();
            for (const auto& callback : instance.mouse_callbacks) {
                callback(xpos, ypos);
            }
        }
    }
} 