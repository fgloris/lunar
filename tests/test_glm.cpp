#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLMTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 设置测试数据
        vec3_a = glm::vec3(1.0f, 0.0f, 0.0f);
        vec3_b = glm::vec3(0.0f, 1.0f, 0.0f);
        mat4_identity = glm::mat4(1.0f);
    }

    glm::vec3 vec3_a;
    glm::vec3 vec3_b;
    glm::mat4 mat4_identity;
};

TEST_F(GLMTest, VectorOperations) {
    // 测试向量点乘
    float dot_product = glm::dot(vec3_a, vec3_b);
    EXPECT_FLOAT_EQ(dot_product, 0.0f);

    // 测试向量叉乘
    glm::vec3 cross_product = glm::cross(vec3_a, vec3_b);
    EXPECT_FLOAT_EQ(cross_product.x, 0.0f);
    EXPECT_FLOAT_EQ(cross_product.y, 0.0f);
    EXPECT_FLOAT_EQ(cross_product.z, 1.0f);

    // 测试向量归一化
    glm::vec3 vec = glm::vec3(3.0f, 4.0f, 0.0f);
    glm::vec3 normalized = glm::normalize(vec);
    EXPECT_FLOAT_EQ(glm::length(normalized), 1.0f);
}

TEST_F(GLMTest, MatrixTransformations) {
    // 测试平移矩阵
    glm::mat4 trans = glm::translate(mat4_identity, glm::vec3(1.0f, 1.0f, 0.0f));
    glm::vec4 point(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 transformed = trans * point;
    EXPECT_FLOAT_EQ(transformed.x, 1.0f);
    EXPECT_FLOAT_EQ(transformed.y, 1.0f);
    EXPECT_FLOAT_EQ(transformed.z, 0.0f);

    // 测试旋转矩阵
    glm::mat4 rotation = glm::rotate(mat4_identity, 
                                   glm::radians(90.0f), 
                                   glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec4 rotated = rotation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_NEAR(rotated.x, 0.0f, 0.0001f);
    EXPECT_NEAR(rotated.y, 1.0f, 0.0001f);
}

TEST_F(GLMTest, ProjectionMatrices) {
    // 测试透视投影矩阵
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
                                          800.0f / 600.0f, 
                                          0.1f, 
                                          100.0f);
    EXPECT_NE(projection, mat4_identity);

    // 测试正交投影矩阵
    glm::mat4 ortho = glm::ortho(-1.0f, 1.0f, 
                                -1.0f, 1.0f, 
                                0.1f, 100.0f);
    EXPECT_NE(ortho, mat4_identity);
}