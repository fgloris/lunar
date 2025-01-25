#include "glm/glm.hpp"

namespace lunar {
class Model {
    public:
    Model();
    ~Model();
    static glm::mat3 getNormalMatrix(glm::mat4 model);
};
}