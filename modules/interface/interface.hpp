#pragma once
#include <map>
#include <string>
#include <functional>
#include "GLFW/glfw3.h"

namespace lunar {
enum class EventType {
    KEY,
    MOUSE_CLICK,
    MOUSE_MOVE,
    MOUSE_SCROLL,
    EMPTY,
};

struct EventIdentifier {
    EventType type;
    int value;
    bool operator<(const EventIdentifier& other) const {
        return type < other.type || (type == other.type && value < other.value);
    }
};

struct Event {
    EventType type;
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
            double xpos;
            double ypos;
        } mouse_move_or_scroll;
    } data;
    [[nodiscard]] std::string what() const;
};

class Interface {
public:
    ~Interface();
    std::map<std::string, std::function<void(Event)>> all_callbacks;
    std::map<EventIdentifier, std::function<void(Event)>> registered_callbacks;

    static Interface& getInstance() {
        static Interface instance;
        return instance;
    }
    void init(const std::string& path, GLFWwindow* window);

    inline static bool initialized = false;
private:
    Interface() = default;
    static int convertKeyNameToGLFW(const std::string& key_name);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
};
}
