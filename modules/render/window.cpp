#include "window.hpp"
#include <yaml-cpp/yaml.h>
#include "interface/interface.hpp"
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

    void Window::init(std::string config_path) {
        YAML::Node config = YAML::LoadFile(config_path);
        this->title = config["window_settings"]["title"].as<std::string>();
        this->default_width = this->width = config["window_settings"]["width"].as<int>();
        this->default_height = this->height = config["window_settings"]["height"].as<int>();
        this->isFullscreen = config["window_settings"]["isFullscreen"].as<bool>();
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
    }

    void Window::initGLAD() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    void Window::fullscreen(const Event& event) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        width = mode->width;
        height = mode->height;
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        glViewport(0, 0, width, height);
        isFullscreen = true;
    }

    void Window::windowed(const Event& event) {
        width = default_width;
        height = default_height;
        glfwSetWindowMonitor(window, nullptr, 0, 0, width, height, GLFW_DONT_CARE);
        isFullscreen = false;
    }
} 