#pragma once
#include <map>
#include <string>
#include <functional>
#include "GLFW/glfw3.h"

namespace lunar {
enum Type {
    KEY,
    MOUSE_CLICK,
    MOUSE_SCROLL
};

struct Event {
    Type type;
    union {
        struct {
            int key;
            int action;
            int scancode;
            int mods;
        } key;
        struct {
            int button;
            double xpos;
            double ypos;
        } mouse_click;
        struct {
            double x_offset;
            double y_offset;
        } mouse_scroll;
    } data;
};

class Interface {
public:
    Interface(std::string path);
    ~Interface();
    std::map<std::string, std::function<void(Event)>> callbacks;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

private:
    int convertKeyNameToGLFW(const std::string& key_name);
};
}