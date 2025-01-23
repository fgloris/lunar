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
    explicit Camera(glm::vec3 camera_pos, float zoom = 45.0f);
    static Camera& getInstance(){ static Camera instance; return instance; }
    [[nodiscard]] glm::mat4 computeViewMatrix() const;
    [[nodiscard]] glm::mat4 computeProjectionMatrix() const;
    [[nodiscard]] glm::vec3 computeTransformMatrix() const;
    void setCameraZoom(const float new_zoom){zoom = new_zoom;}
    private:
    float zoom;
    glm::vec3 camera_pos, camera_front, camera_direction, camera_up, camera_right;
};
}