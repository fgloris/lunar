#pragma once
#include <glm/glm.hpp>
#include <stdexcept>

namespace lunar{

struct Event;
class Interface;

class Camera{
    public:
    Camera();
    explicit Camera(glm::vec3 camera_pos, float zoom = 45.0f);
    static Camera& getInstance(){ static Camera instance; return instance; }
    [[nodiscard]] glm::mat4 computeViewMatrix() const;
    [[nodiscard]] glm::mat4 computeProjectionMatrix() const;
    [[nodiscard]] glm::vec3 computeTransformMatrix() const;
    void resetZoom() {focus = 45.0f;}
    void zoom(const Event& event);
    void rotate(const Event& event);
    void moveForward(const Event& event);
    void moveBackward(const Event& event);
    void moveLeft(const Event& event);
    void moveRight(const Event& event);
    void moveUp(const Event& event);
    void moveDown(const Event& event);
    void registerCallback(Interface& interface);
    [[nodiscard]] glm::vec3 getPosition() const {return camera_pos;}

    inline static float zoom_speed = 2.0f;
    inline static float rotate_speed = 0.001f;
    inline static float move_speed = 1.0f;
    private:
    double focus;
    glm::vec3 camera_pos, camera_direction, camera_up, camera_right;
};
}