#include "glm/glm.hpp"
#include <string>
#include <map>

namespace lunar {
struct Material;
class Model {
    public:
    Model();
    ~Model();
    static glm::mat3 getNormalMatrix(glm::mat4 model);
    static std::map<std::string, Material> materials;
};

}