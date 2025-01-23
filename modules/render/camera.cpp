#include "camera.hpp"
#include "window.hpp"
#include "interface/interface.hpp"

namespace lunar{
    Camera::Camera(glm::vec3 camera_pos, float zoom):zoom(zoom),
    camera_pos(camera_pos), camera_front(glm::vec3(0.0f, 0.0f, -1.0f)){
        camera_direction = glm::normalize(camera_pos - camera_front);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera_right = glm::normalize(glm::cross(up, camera_direction));
        camera_up = glm::normalize(glm::cross(camera_direction, camera_right));
    }
    glm::mat4 Camera::computeViewMatrix() const {
        return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    }

    glm::mat4 Camera::computeProjectionMatrix() const{
        if (!Window::initialized){
            throw std::runtime_error("Window not initialized");
        }else {
            Window& w = Window::getInstance();
            return glm::perspective(glm::radians(zoom), static_cast<float>(w.getWidth()) / static_cast<float>(w.getHeight()), 0.1f, 100.0f);
        }
    }

    void Camera::Zoom(Event event){
        zoom += event.data.mouse_move_or_scroll.ypos * zoom_speed;
    }

    void Camera::Move(Event event){
        switch (event.type){
            case EventType::MOUSE_MOVE:
                camera_direction = glm::normalize(camera_pos - camera_front);
                camera_pos += (float)(event.data.mouse_move_or_scroll.xpos * move_speed) * camera_direction;
                break;
            case EventType::MOUSE_SCROLL:
                zoom += event.data.mouse_move_or_scroll.ypos * zoom_speed;
                break;
            case EventType::KEY:
            case EventType::MOUSE_CLICK:
                camera_pos += move_speed * camera_direction;
                break;
            default:
                break;
        }
    }
}