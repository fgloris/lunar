#pragma once
#include "window.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace lunar{
class Camera{
    public:
    Camera();
    ~Camera();
    static Camera& getInstance();
    glm::mat4 getViewMatrix() const {return view;}
    glm::mat4 getProjectionMatrix() const {return projection;}
    glm::mat4 getTransform() { return transform = projection * view; }
    private:
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 transform;
};
}