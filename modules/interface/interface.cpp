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
    default: return "<unknown event>";
    }
}

void Interface::bindCallbacks(const std::string& config_path, GLFWwindow* window) {
    try {
        YAML::Node config = YAML::LoadFile(config_path);
        
        if (config["keyboard_and_mouse_bindings"]) {
            for (const auto& binding : config["keyboard_and_mouse_bindings"]) {
                std::string key_name = binding["key"].as<std::string>();
                std::string callback_name = binding["callback"].as<std::string>();

                if (callback_name == "empty") {
                    continue;
                }

                const EventIdentifier event_identifier = convertKeyNameToEventIndetifier(key_name);
                
                if (all_callbacks.find(callback_name) != all_callbacks.end()) {
                    registered_callbacks[event_identifier] = all_callbacks[callback_name];
                } else {
                    throw std::runtime_error("Callback not found: " + callback_name);
                }
            }
        }
        
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading control config: " << e.what() << std::endl;
    }
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    //glfwSetScrollCallback(window, mouseScrollCallback);
    //glfwSetCursorPosCallback(window, mouseMoveCallback);
    bound = true;
}

void Interface::registerCallback(const std::string& name, std::function<void(Event)> callback) {
    all_callbacks[name] = callback;
}

void Interface::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto& registered_callbacks = Interface::getInstance().registered_callbacks;
    Event event = {EventType::KEY, {key, scancode, action, mods}};
    if (registered_callbacks.find(key) != registered_callbacks.end()) {
        registered_callbacks[key](event);
    }
}

void Interface::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto& registered_callbacks = Interface::getInstance().registered_callbacks;
    Event event = {EventType::MOUSE_CLICK, {button, action, mods}};
    if (registered_callbacks.find(button) != registered_callbacks.end()) {
        registered_callbacks[button](event);
    }
}

Interface::~Interface() {
    registered_callbacks.clear();
    all_callbacks.clear();
}

}