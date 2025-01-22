#include "camera.hpp"

namespace lunar{
    Camera::Camera(glm::vec3 camera_pos, float zoom):zoom(zoom),
    camera_pos(camera_pos), camera_front(glm::vec3(0.0f, 0.0f, -1.0f)){
        camera_direction = glm::normalize(camera_pos - camera_front);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera_right = glm::normalize(glm::cross(up, camera_direction));
        camera_up = glm::normalize(glm::cross(camera_direction, camera_right));
        view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    }
    Camera::~Camera() {}
    glm::mat4 Camera::getProjectionMatrix(){
        if (!Window::initialized){
            throw std::runtime_error("Window not initialized");
        }else{
            Window& w = Window::getInstance();
            projection = glm::perspective(glm::radians(zoom), (float)w.getWidth() / (float)w.getHeight(), 0.1f, 100.0f);
        }
        return projection;
    }
}