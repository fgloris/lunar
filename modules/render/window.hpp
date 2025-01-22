#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <vector>

namespace lunar {
    class Window {
        using KeyCallbackFunc = std::function<void(int key, int action)>;
        using MouseCallbackFunc = std::function<void(double xpos, double ypos)>;
    public:
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        static Window& getInstance() {
            static Window instance;
            return instance;
        }
        
        void init(int width, int height, const std::string& title, bool isFullscreen = false);

        bool shouldClose() const { return glfwWindowShouldClose(window); }
        void swapBuffers() { glfwSwapBuffers(window); }

        void pollEvents() { glfwPollEvents(); processInput(); }
        void registerKeyCallback(KeyCallbackFunc callback) { key_callbacks.push_back(callback); }
        void registerMouseCallback(MouseCallbackFunc callback) { mouse_callbacks.push_back(callback); }

        GLFWwindow* getHandle() const { return window; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }

        inline static bool initialized = false;
    private:
        std::vector<KeyCallbackFunc> key_callbacks;
        std::vector<MouseCallbackFunc> mouse_callbacks;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        
        Window() = default;
        ~Window();
        void initGLFW();
        void initWindow();
        void initGLAD();
        void processInput();

        GLFWwindow* window{nullptr};
        int width{0}, height{0};
        std::string title;
        bool isFullscreen{false};
    };
} 