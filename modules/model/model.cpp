#include "model.hpp"

namespace lunar {

glm::mat3 Model::getNormalMatrix(glm::mat4 model) {
    return glm::mat3(glm::transpose(glm::inverse(model)));
}

}