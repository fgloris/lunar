#include "interface.hpp"
#include "fmt/format.h"
#include <yaml-cpp/yaml.h>
#include <iostream>

namespace lunar {
[[nodiscard]] std::string Event::what() const{
    switch (type)
    {
    case EventType::KEY:
        return fmt::format("<keyboard: key:{}, action:{}, scancode:{}, mods:{}>",
                                data.key.key,data.key.action,data.key.scancode,data.key.mods);
    case EventType::MOUSE_CLICK:
        return fmt::format("<mouse click: button:{}, x:{}, y:{}>",
                                data.mouse_click.button,data.mouse_click.xpos,data.mouse_click.ypos);
    case EventType::MOUSE_MOVE:
        return fmt::format("<mouse move: x:{}, y:{}>",
                                data.mouse_move_or_scroll.xpos,data.mouse_move_or_scroll.ypos);
    case EventType::MOUSE_SCROLL:
        return fmt::format("<mouse scroll: x:{}, y:{}>",
                                data.mouse_move_or_scroll.xpos,data.mouse_move_or_scroll.ypos);
    default:
        return "<unknown event>";
    }
}

void Interface::init(const std::string& config_path, GLFWwindow* window) {
    try {
        YAML::Node config = YAML::LoadFile(config_path);
        
        if (config["keyboard_bindings"]) {
            for (const auto& binding : config["keyboard_bindings"]) {
                std::string key_name = binding["key"].as<std::string>();
                std::string callback_name = binding["callback"].as<std::string>();

                const int key_value = convertKeyNameToGLFW(key_name);
                
                EventIdentifier event_identifier{EventType::KEY, {key_value}};
                registered_callbacks[event_identifier] = all_callbacks[callback_name];
            }
        }
        
        if (config["mouse_bindings"]) {
            for (const auto& binding : config["mouse_bindings"]) {
                std::string action = binding["action"].as<std::string>();
                std::string callback_name = binding["callback"].as<std::string>();
                
                const int action_value = convertKeyNameToGLFW(action);

                EventIdentifier event_identifier{EventType::MOUSE_CLICK, {action_value}};
                registered_callbacks[event_identifier] = all_callbacks[callback_name];
            }
        }
        
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading control config: " << e.what() << std::endl;
    }
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    initialized = true;
}

int Interface::convertKeyNameToGLFW(const std::string& key_name) {
    static const std::map<std::string, int> key_map = {
        {"GLFW_KEY_A", GLFW_KEY_A},
        {"GLFW_KEY_B", GLFW_KEY_B},
        {"GLFW_KEY_C", GLFW_KEY_C},
        {"GLFW_KEY_D", GLFW_KEY_D},
        {"GLFW_KEY_E", GLFW_KEY_E},
        {"GLFW_KEY_F", GLFW_KEY_F},
        {"GLFW_KEY_G", GLFW_KEY_G},
        {"GLFW_KEY_H", GLFW_KEY_H},
        {"GLFW_KEY_I", GLFW_KEY_I},
        {"GLFW_KEY_J", GLFW_KEY_J},
        {"GLFW_KEY_K", GLFW_KEY_K},
        {"GLFW_KEY_L", GLFW_KEY_L},
        {"GLFW_KEY_M", GLFW_KEY_M},
        {"GLFW_KEY_N", GLFW_KEY_N},
        {"GLFW_KEY_O", GLFW_KEY_O},
        {"GLFW_KEY_P", GLFW_KEY_P},
        {"GLFW_KEY_Q", GLFW_KEY_Q},
        {"GLFW_KEY_R", GLFW_KEY_R},
        {"GLFW_KEY_S", GLFW_KEY_S},
        {"GLFW_KEY_T", GLFW_KEY_T},
        {"GLFW_KEY_U", GLFW_KEY_U},
        {"GLFW_KEY_V", GLFW_KEY_V},
        {"GLFW_KEY_W", GLFW_KEY_W},
        {"GLFW_KEY_X", GLFW_KEY_X},
        {"GLFW_KEY_Y", GLFW_KEY_Y},
        {"GLFW_KEY_Z", GLFW_KEY_Z},
        {"GLFW_KEY_SPACE", GLFW_KEY_SPACE},
        {"GLFW_KEY_LEFT_SHIFT", GLFW_KEY_LEFT_SHIFT},
        {"GLFW_KEY_ESCAPE", GLFW_KEY_ESCAPE},
        {"GLFW_KEY_ENTER", GLFW_KEY_ENTER},
        {"GLFW_KEY_BACKSPACE", GLFW_KEY_BACKSPACE},
        {"GLFW_KEY_TAB", GLFW_KEY_TAB},
        {"GLFW_KEY_LEFT_CONTROL", GLFW_KEY_LEFT_CONTROL},
        {"GLFW_KEY_RIGHT_CONTROL", GLFW_KEY_RIGHT_CONTROL},
        {"GLFW_KEY_LEFT_ALT", GLFW_KEY_LEFT_ALT},
        {"GLFW_KEY_RIGHT_ALT", GLFW_KEY_RIGHT_ALT},
        {"GLFW_KEY_LEFT_SUPER", GLFW_KEY_LEFT_SUPER},
        {"GLFW_KEY_RIGHT_SUPER", GLFW_KEY_RIGHT_SUPER},
        {"GLFW_KEY_LEFT_BRACKET", GLFW_KEY_LEFT_BRACKET},
        {"GLFW_KEY_RIGHT_BRACKET", GLFW_KEY_RIGHT_BRACKET},
        {"GLFW_KEY_SEMICOLON", GLFW_KEY_SEMICOLON},
        {"GLFW_KEY_APOSTROPHE", GLFW_KEY_APOSTROPHE},
        {"GLFW_KEY_GRAVE_ACCENT", GLFW_KEY_GRAVE_ACCENT},
        {"GLFW_KEY_COMMA", GLFW_KEY_COMMA},
        {"GLFW_KEY_PERIOD", GLFW_KEY_PERIOD},
        {"GLFW_KEY_SLASH", GLFW_KEY_SLASH},
        {"GLFW_KEY_BACKSLASH", GLFW_KEY_BACKSLASH},

        {"GLFW_MOUSE_BUTTON_LEFT", GLFW_MOUSE_BUTTON_LEFT},
        {"GLFW_MOUSE_BUTTON_RIGHT", GLFW_MOUSE_BUTTON_RIGHT},
        {"GLFW_MOUSE_BUTTON_MIDDLE", GLFW_MOUSE_BUTTON_MIDDLE},
        {"GLFW_MOUSE_BUTTON_4", GLFW_MOUSE_BUTTON_4},
        {"GLFW_MOUSE_BUTTON_5", GLFW_MOUSE_BUTTON_5},
        {"GLFW_MOUSE_BUTTON_6", GLFW_MOUSE_BUTTON_6},
        {"GLFW_MOUSE_BUTTON_7", GLFW_MOUSE_BUTTON_7},
        {"GLFW_MOUSE_BUTTON_8", GLFW_MOUSE_BUTTON_8},
    };
    
    auto it = key_map.find(key_name);
    if (it != key_map.end()) {
        return it->second;
    }
    
    std::cerr << "Warning: Unknown key name: " << key_name << std::endl;
    return -1;
}

Interface::~Interface() {
    registered_callbacks.clear();
    all_callbacks.clear();
}

}