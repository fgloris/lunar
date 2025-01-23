#include "interface.hpp"
#include <yaml-cpp/yaml.h>
#include <iostream>

namespace lunar {

Interface::Interface(const std::string& config_path) {
    try {
        YAML::Node config = YAML::LoadFile(config_path);
        
        if (config["keyboard_bindings"]) {
            for (const auto& binding : config["keyboard_bindings"]) {
                std::string key_name = binding["key"].as<std::string>();
                std::string callback_name = binding["callback"].as<std::string>();
                
                int key_value = convertKeyNameToGLFW(key_name);
                
                Event event;
                event.type = EventType::KEY;
                event.data.key.key = key_value;
                
                callbacks[callback_name] = [](Event e) {
                    std::cout << "Key event triggered: " << e.data.key.key << std::endl;
                };
            }
        }
        
        if (config["mouse_bindings"]) {
            for (const auto& binding : config["mouse_bindings"]) {
                std::string action = binding["action"].as<std::string>();
                std::string callback_name = binding["callback"].as<std::string>();
                
                Event event;
                if (action == "look_around") {
                    event.type = EventType::MOUSE_CLICK;
                } else if (action == "scroll") {
                    event.type = EventType::MOUSE_SCROLL;
                }
                
                callbacks[callback_name] = [](Event e) {
                    std::cout << "Mouse event triggered: " << e.what() << std::endl;
                };
            }
        }
        
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading control config: " << e.what() << std::endl;
    }
}

int Interface::convertKeyNameToGLFW(const std::string& key_name) {
    static const std::map<std::string, int> key_map = {
        {"GLFW_KEY_W", GLFW_KEY_W},
        {"GLFW_KEY_A", GLFW_KEY_A},
        {"GLFW_KEY_S", GLFW_KEY_S},
        {"GLFW_KEY_D", GLFW_KEY_D},
        {"GLFW_KEY_Q", GLFW_KEY_Q},
        {"GLFW_KEY_E", GLFW_KEY_E},
        {"GLFW_KEY_SPACE", GLFW_KEY_SPACE},
        {"GLFW_KEY_LEFT_SHIFT", GLFW_KEY_LEFT_SHIFT},
        {"GLFW_KEY_ESCAPE", GLFW_KEY_ESCAPE}
    };
    
    auto it = key_map.find(key_name);
    if (it != key_map.end()) {
        return it->second;
    }
    
    std::cerr << "Warning: Unknown key name: " << key_name << std::endl;
    return -1;
}

Interface::~Interface() {
    callbacks.clear();
}

}