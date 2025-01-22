#pragma once
#include "window.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

namespace lunar{
class Camera{
    public:
    Camera();
    Camera(glm::vec3 camera_pos, float zoom = 45.0f);
    ~Camera();
    static Camera& getInstance(){ static Camera instance; return instance; }
    glm::mat4 getViewMatrix() const {return view;}
    glm::mat4 getProjectionMatrix();
    void setCameraZoom(float new_zoom){zoom = new_zoom;}
    private:
    float zoom;
    glm::vec3 camera_pos, camera_front, camera_direction, camera_up, camera_right;
    glm::mat4 view, projection;
};
}